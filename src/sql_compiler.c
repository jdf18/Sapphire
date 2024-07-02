#include "sql_compiler.h"

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "../libs/Topaz/topaz.h"

#include "dynamic_arrays.h"
#include "string.h"
#include "utils.h"

// * Lexical Analysis

typedef enum {
    TOKEN_CREATE,
    TOKEN_DROP,
    TOKEN_SELECT,
    TOKEN_INSERT,
    TOKEN_UPDATE,
    TOKEN_DELETE,
    TOKEN_TABLE,
    TOKEN_INTO,
    TOKEN_IDENTIFIER,
    TOKEN_QUOTE,
    TOKEN_DISTINCT,
    TOKEN_ALL,
    TOKEN_ASTERISK,
    TOKEN_PERIOD,
    TOKEN_UNKNOWN,
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

Token create_token(TokenType type, char* value) {
    Token token;
    token.type = type;
    token.value = value;
    return token;
}

DEFINE_DYNAMIC_ARRAY(Token, TokenArray)

void get_length_of_quote(const char* p, const char** start, const char** end, size_t* length) {
    *start = p + 1;
    // Loop until the ending single quote to determine the length of the string
    *length = 0;
    while (true) {
        if (*p == '"') {
            if (*(p+1) == '"') {
                p += 2;
                (*length)--;
            } else break;
        } else p++;
    }
    *end = p;
    *length += *end - *start;
}

void copy_quote(const char* source, const char* start, const char* end, char * destination) {
    source = start;
    while (source < end) {
        *destination = *source;
        destination++;
        source++;
        if (*source == '"') {
            source++; // Additional increment on source
        }
    }
    *destination = '\0';
}

void lexer(const char * sql, TokenArray_t *tokens) {
    // initialise the token array
    TokenArray_init(tokens);

    // Create another pointer to index into the string
    const char * p = sql;

    while (*p != '\0') {
        if (strncmp(p, "CREATE", 6) == 0) {
            TokenArray_add(tokens, create_token(TOKEN_CREATE, NULL));
            p += 6;
        } else if (strncmp(p, "DROP", 4) == 0) {
            TokenArray_add(tokens, create_token(TOKEN_DROP, NULL));
            p += 4;
        } else if (strncmp(p, "SELECT", 6) == 0) {
            TokenArray_add(tokens, create_token(TOKEN_SELECT, NULL));
            p += 6;
        } else if (strncmp(p, "INSERT", 6) == 0) {
            TokenArray_add(tokens, create_token(TOKEN_INSERT, NULL));
            p += 6;
        } else if (strncmp(p, "UPDATE", 6) == 0) {
            TokenArray_add(tokens, create_token(TOKEN_UPDATE, NULL));
            p += 6;
        } else if (strncmp(p, "DELETE", 6) == 0) {
            TokenArray_add(tokens, create_token(TOKEN_DELETE, NULL));
            p += 6;
        } else if (strncmp(p, "TABLE", 5) == 0) {
            TokenArray_add(tokens, create_token(TOKEN_TABLE, NULL));
            p += 5;
        } else if (strncmp(p, "INTO", 4) == 0) {
            TokenArray_add(tokens, create_token(TOKEN_INTO, NULL));
            p += 4;
        } else if (strncmp(p, "DISTINCT", 8) == 0) {
            TokenArray_add(tokens, create_token(TOKEN_DISTINCT, NULL));
            p += 8;
        } else if (strncmp(p, "ALL", 3) == 0) {
            TokenArray_add(tokens, create_token(TOKEN_ALL, NULL));
            p += 3;
        } else if (*p == '*') {
            TokenArray_add(tokens, create_token(TOKEN_ASTERISK, NULL));
            p += 1;
        } else if (*p == '.') {
            TokenArray_add(tokens, create_token(TOKEN_PERIOD, NULL));
            p += 1;
        } else if (*p == '"') {
            // Start of the quoted name after the double quote.
            p++;

            // Find the length of the quote
            const char * start;
            const char* end;
            size_t length;
            get_length_of_quote(p, &start, &end, &length);

            // Allocate memory and copy the string over
            char * output = (char*)malloc(length + 1); // +1 for \0
            if (output == NULL) {
                LOG_ERROR("Memory allocation for quoted name token failed.");
                return; // TODO: Get an ERROR message for this
            }
            copy_quote(p, start, end, output);

            // Return the token
            TokenArray_add(tokens, create_token(TOKEN_QUOTE, output));
            p++;

        } else if (isalnum(*p)) {
            // Start of an identifier. Identifiers must start with alnum and can contain alnums or underscores
            const char* start = p;
            // Loop until an invalid character found
            while (isalnum(*p) || *p == '_') p++;
            size_t length = p - start;
            char* identifier = my_strndup(start, (int) length);
            TokenArray_add(tokens, create_token(TOKEN_IDENTIFIER, identifier));
        } else if (isspace(*p)) {
            p++;
        } else {
            TokenArray_add(tokens, create_token(TOKEN_UNKNOWN, my_strndup(p, 1)));
            p++;
        }
    }
}

// * Syntax Analysis

typedef enum {
    NODE_SELECT,
    NODE_INSERT,
    NODE_UPDATE,
    NODE_DELETE,
    NODE_CREATE_TABLE,
    NODE_DROP_TABLE,
    NODE_SELECT_EXPRESSION,
    NODE_NAME
} NodeType;

// Fundamental nodes

typedef struct {
    char * name;
    // This can either be an identifier, or a quote
} NodeName;

typedef struct {

} NodeTerm;

// Expressions

typedef enum {
    NODE_SELECT_EXPRESSION_WILDCARD,
    NODE_SELECT_EXPRESSION_FAMILY_WILDCARD,
    NODE_SELECT_EXPRESSION_TERM,
    NODE_SELECT_EXPRESSION_TERM_ALIAS
} NodeSelectExpressionType;

typedef struct {
    NodeSelectExpressionType select_expression_type;
    union {
        NodeName familyName;
        struct {
            NodeTerm* term;
            NodeName alias;
        };
    };
} NodeSelectExpression;

// SQL Instructions
typedef struct {
    bool is_distinct; // Filters out duplicate results (Defaults false to ALL)
    NodeSelectExpression* select_expressions;
    size_t select_expressions_count;
} NodeSelect;

typedef struct {
    char* table_name;
    char ** columns;
    char ** values;
    size_t column_count;
    size_t value_count;
} NodeInsert;

typedef struct {
    char* table_name;
} NodeUpdate;

typedef struct {
    char* table_name;
} NodeDelete;

typedef struct {
    char* table_name;
} NodeCreateTable;

typedef struct {
    char* table_name;
} NodeDropTable;

typedef struct {
    NodeType type;
    union {
        NodeSelect select_node;
        NodeInsert insert_node;
        NodeUpdate update_node;
        NodeDelete delete_node;
        NodeCreateTable create_table_node;
        NodeDropTable drop_table_node;
        NodeSelectExpression select_expression;
        NodeName name_none;
    };
} ASTNode;


ASTNode* allocate_node() {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) {
        LOG_ERROR("Allocation of select node failed during SQL compilation");
        return NULL;
    }
    return node;
}

void update_name_node(NodeName * node, char * name) {
    node->name = strdup(name);
}

void update_select_expression_node(NodeSelectExpression* node, NodeSelectExpressionType type, NodeName* family_name, NodeTerm* term, NodeName* alias) {
    node->select_expression_type = type;
    switch (type) {
        case NODE_SELECT_EXPRESSION_FAMILY_WILDCARD:
            update_name_node(&(node->familyName), family_name->name);
        case NODE_SELECT_EXPRESSION_WILDCARD:
            break;
        case NODE_SELECT_EXPRESSION_TERM_ALIAS:
            update_name_node(&(node->alias), alias->name);
        case NODE_SELECT_EXPRESSION_TERM:
            node->term = term;
            break;
    }
}

void update_select_node(ASTNode *node, bool is_distinct, NodeSelectExpression* select_expressions, size_t select_expressions_count) {
    node->type = NODE_SELECT;
    node->select_node.is_distinct = is_distinct;
    node->select_node.select_expressions = select_expressions;
    node->select_node.select_expressions_count = select_expressions_count;
}

void update_insert_node(ASTNode * node, const char * table_name, char ** columns, char ** values, size_t column_count, size_t value_count) {
    node->type = NODE_INSERT;
    node->insert_node.table_name = strdup(table_name);
    node->insert_node.columns = columns;
    node->insert_node.values = values;
    node->insert_node.column_count = column_count;
    node->insert_node.value_count = value_count;
}

// TODO: The rest of these node generators

// Parsing logic

bool is_parse_name_possible(Token** p) {
    switch ((*p)->type) {
        case TOKEN_QUOTE:
        case TOKEN_IDENTIFIER:
            return true;
        default: return false;
    }
}

SQLCompilationResult parse_name(Token** p, NodeName* name_node) {
    switch ((*p)->type) {
        case TOKEN_QUOTE:
        case TOKEN_IDENTIFIER:
            update_name_node(name_node, (*p)->value);
            return SQL_COMPILATION_SUCCESS;
        default:
            return SQL_COMPILATION_INVALID_TOKEN;
    }
}

SQLCompilationResult parse_select_expression(Token** p, NodeSelectExpression* select_expression_node) {
    if ((*p)->type == TOKEN_ASTERISK) {
        update_select_expression_node(select_expression_node, NODE_SELECT_EXPRESSION_WILDCARD, NULL, NULL, NULL);
        return SQL_COMPILATION_SUCCESS;
    }

    if (is_parse_name_possible(p) && ((*p)+1)->type == TOKEN_PERIOD && ((*p)+2)->type == TOKEN_ASTERISK) {
        NodeName* family_name;
        family_name = malloc(sizeof(NodeName));
        parse_name(p, family_name);
        update_select_expression_node(select_expression_node, NODE_SELECT_EXPRESSION_FAMILY_WILDCARD, family_name, NULL, NULL);
        return SQL_COMPILATION_SUCCESS;
    }

    // Term and possible alias, optional AS keyword

    return SQL_COMPILATION_NO_VALID_PATHS;
}

SQLCompilationResult parse_select_statement(Token** p, ASTNode* ast) {
    // * SELECT keyword
    if ((*p)->type != TOKEN_SELECT) {
        LOG_WARN("Parsing select statement, does not begin with a SELECT token. ");
        return SQL_COMPILATION_MISSING_KEYWORD;
    }
    (*p)++;

    // * Optional DISTINCT/ALL keyword (defaults to all)
    bool is_distinct = false;
    if ((*p)->type == TOKEN_DISTINCT) {
        is_distinct = true;
        (*p)++;
    } else if ((*p)->type == TOKEN_ALL) {
        (*p)++;
    }

    NodeSelectExpression * se;
    se = malloc(sizeof(NodeSelectExpression));
    // * SelectExpression required
    parse_select_expression(p, se);
    // , ...

    update_select_node(ast, is_distinct, se, 1);


    return SQL_COMPILATION_SUCCESS;
}

SQLCompilationResult parse(Token* tokens, size_t length, ASTNode* ast) {
    Token* p = tokens;

    // Parse into an SQL statement
    SQLCompilationResult result;
    switch (p->type) {
        case TOKEN_SELECT:
            result = parse_select_statement(&p, ast);
            break;
        default: // If did not match any statements, return an error
            return SQL_COMPILATION_MISSING_KEYWORD;
    }
    // If an error was produced during statement compilation, return it.
    if (result != SQL_COMPILATION_SUCCESS) {
        return result;
    }

    // Check the correct number of tokens have been parsed before returning success.
    if (p == tokens + length) {
        return SQL_COMPILATION_SUCCESS;
    }
    // Tokens remaining to be parsed so error.
    return SQL_COMPILATION_REMAINING_TOKENS;
}


SQLCompilationResult compile_sql_statement(const char* input, Instruction* instruction) {
    SQLCompilationResult result;

    // * Lexical Analysis
    TokenArray_t tokens;
    lexer(input, &tokens);
    // Lexing can not fail

    // * Syntax Analysis
    ASTNode* ast = allocate_node();
    result = parse(tokens.data, tokens.size, ast);
    if (result != SQL_COMPILATION_SUCCESS) {
        return result;
    }

    // * Semantic Analysis

    // * Intermediate Representation Generation

    // * Compilation completed successfully
    return SQL_COMPILATION_SUCCESS;
}