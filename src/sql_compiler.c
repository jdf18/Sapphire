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
    TOKEN_DISTINCT,
    TOKEN_ALL,
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
        } else if (isalnum(*p)) {
            // Start of an identifier. Identifiers must start with alnum and can contain alnums or underscores
            const char* start = p;
            // Loop until an invalid character found
            while (isalnum(*p) || *p == '_') p++;
            size_t length = p - start;
            char* identifier = my_strndup(start, (int) length);
            TokenArray_add(tokens, create_token(TOKEN_IDENTIFIER, identifier));
            //free(identifier);
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
    NODE_SELECT_EXPRESSION
} NodeType;

// Expressions

typedef struct {
    bool is_wildcard;
} NodeSelectExpression;

// SQL Instructions
typedef struct {
    char* table_name;
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
        NodeSelectExpression nodeSelectExpression;
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

ASTNode* update_select_expression_node(ASTNode* node, bool is_wildcard) {
    node->type = NODE_SELECT_EXPRESSION;
    node->nodeSelectExpression.is_wildcard = is_wildcard;
    return node;
}

ASTNode* update_select_node(ASTNode *node, const char * table_name) {
    node->type = NODE_SELECT;
    node->select_node.table_name = strdup(table_name);

    return node;
}

ASTNode* update_insert_node(ASTNode* node, const char * table_name, char ** columns, char ** values, size_t column_count, size_t value_count) {
    node->type = NODE_INSERT;
    node->insert_node.table_name = strdup(table_name);
    node->insert_node.columns = columns;
    node->insert_node.values = values;
    node->insert_node.column_count = column_count;
    node->insert_node.value_count = value_count;

    return node;
}

// TODO: The rest of these node generators

// Parsing logic

SQLCompilationResult parse_select_expression(Token** p, ASTNode* ast) {
    return SQL_COMPILATION_SUCCESS;
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

    // * SelectExpression required
    parse_select_expression(p, ast);
    // , ...


    ast = update_select_node(ast, "test");

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