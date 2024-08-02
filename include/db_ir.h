#ifndef SAPPHIRE_DB_IR_H
#define SAPPHIRE_DB_IR_H

#include <stdbool.h>

#include "../libs/Topaz/topaz.h"

#include "database.h"

// * === EXPRESSIONS ===

typedef struct Expression Expression;
typedef struct ExpressionSummand ExpressionSummand;

// Copy of stored values in _meta_types table
typedef enum {
    EXPRESSION_VALUE_TYPE_STRING,
    EXPRESSION_VALUE_TYPE_INTEGER,
    EXPRESSION_VALUE_TYPE_BOOLEAN,
    EXPRESSION_VALUE_TYPE_NULL
} ExpressionValueType;

typedef struct {
    ExpressionValueType type;
    union {
        char* string;
        int integer;
        bool boolean;
    } value;
} ExpressionValue;

typedef enum {
    EXPRESSION_TERM_TYPE_VALUE,
    EXPRESSION_TERM_TYPE_PARAMETER,
    EXPRESSION_TERM_TYPE_SUMMAND,
    //todo: function, case, columnReference, RVC
} ExpressionTermType;

typedef struct {
    ExpressionTermType type;
    union {
        ExpressionValue* value;
        int param_index;
        ExpressionSummand* summand;
    };
} ExpressionTerm;

typedef struct {
    size_t num_terms;
    struct {
        enum {MULTIPLY, DIVIDE} operation;
        ExpressionTerm term;
    }** terms;
} ExpressionFactor;

struct ExpressionSummand{
    size_t num_factors;
    struct {
        bool negative;
        ExpressionFactor factor;
    }** factors;
};

typedef struct {
    // todo: lists of literals, SELECT statements, etc.
} ExpressionList;

typedef enum {
    EXPRESSION_CONDITION_TYPE_OPERAND,
    EXPRESSION_CONDITION_TYPE_OPERAND_COMPARE,
    EXPRESSION_CONDITION_TYPE_OPERAND_IS_IN,
    EXPRESSION_CONDITION_TYPE_OPERAND_NOT_IN,
    EXPRESSION_CONDITION_TYPE_OPERAND_IS_NULL,
    EXPRESSION_CONDITION_TYPE_OPERAND_NOT_NULL,
    EXPRESSION_CONDITION_TYPE_EXPRESSION,
    EXPRESSION_CONDITION_TYPE_NOT_EXPRESSION,
} ExpressionConditionType;

bool is_expression_condition_negative(ExpressionConditionType type) {
    switch (type) {
        case EXPRESSION_CONDITION_TYPE_OPERAND:
        case EXPRESSION_CONDITION_TYPE_OPERAND_COMPARE:
        case EXPRESSION_CONDITION_TYPE_OPERAND_IS_IN:
        case EXPRESSION_CONDITION_TYPE_OPERAND_IS_NULL:
        case EXPRESSION_CONDITION_TYPE_EXPRESSION:
            return false;
        case EXPRESSION_CONDITION_TYPE_OPERAND_NOT_IN:
        case EXPRESSION_CONDITION_TYPE_OPERAND_NOT_NULL:
        case EXPRESSION_CONDITION_TYPE_NOT_EXPRESSION:
            return true;
        default:
            LOG_WARN("Switch statement is not exhastive (db_ir.is_expression_condition_negative)");
            return false;
    }
}

typedef enum {
    EXPRESSION_COMPARISON_NOT_EQUAL,
    EXPRESSION_COMPARISON_LESS_OR_EQUAL,
    EXPRESSION_COMPARISON_GREATER_OR_EQUAL,
    EXPRESSION_COMPARISON_EQUAL,
    EXPRESSION_COMPARISON_LESS,
    EXPRESSION_COMPARISON_GREATER,
} ExpressionComparisonOperator;

typedef struct {
    ExpressionConditionType type;
    union {
        struct {
            Expression* expression;
        } expression_condition_type;
        struct {
            ExpressionSummand* summand;
            union {
                struct {
                    ExpressionComparisonOperator comparator;
                    ExpressionSummand* right_summand;
                } operand_comparison_condition_type;
                struct {
                    ExpressionList* list;
                } operand_in_condition_type;
            };
        } operand_condition_type;
    };
} ExpressionCondition;

typedef struct {
    // condition1 AND condition2 AND ...
    size_t num_conditions;
    ExpressionCondition** conditions;
} ExpressionAndCondition;

struct Expression {
    // and_condition1 OR and_condition2 OR ...
    size_t num_and_conditions;
    ExpressionAndCondition** and_conditions;
};

typedef struct {
    int table_uid;
    int column_uid;
} ColumnReference;

typedef struct {
    size_t num_columns;
    ColumnReference* columns;
} ColumnsReference;

// * === TABLES ===

typedef enum {
    PRIMARY_KEY = 1 << 0, // 1,   0b1
    AUTO_ASSIGN = 1 << 1, // 2,   0b10
    UNIQUE      = 1 << 2, // 4,   0b100
    REQUIRED    = 1 << 3, // 8,   0b1000
    INDEXED     = 1 << 4, // 16,  0b10000
    REFERENCE   = 1 << 5, // 32,  0b100000
    HAS_DEFAULT = 1 << 6, // 64,  0b1000000
} TableColumnFlags;

typedef struct {
    size_t num_columns;
    int* column_id[]; // Pointer to an array of ints
} UniqueRule;

typedef struct {
    int column_uid;
    char name[DATABASE_COLUMN_NAME_MAX_SIZE];
    int column_flags; // See TableColumnFlags
} TableColumnDetails;

typedef struct {
    int table_uid;
    char* name;
    size_t num_columns;
    TableColumnDetails* columns; // Pointer to an array of columns
    size_t num_pk_columns;
    int* pk_columns; // Pointer to an array of ints
    size_t num_unique_rules;
    UniqueRule* unique_rules; // Pointer to an array of rules
} TableDetails;


// * === INSTRUCTIONS ===

typedef enum {
    INSTRUCTION_UNKNOWN,
    INSTRUCTION_INSPECT_TABLE,
    INSTRUCTION_CREATE_TABLE,
    INSTRUCTION_MODIFY_TABLE,
    INSTRUCTION_DELETE_TABLE,
    INSTRUCTION_COPY_TABLE,
    INSTRUCTION_SELECT_DATA,
    INSTRUCTION_INSERT_DATA,
    INSTRUCTION_UPDATE_DATA,
    INSTRUCTION_DELETE_DATA
} InstructionType;

typedef struct {
    InstructionType instruction_type;
} InstructionHeader;

typedef struct {
    InstructionHeader instruction_header;
    bool is_wildcard;
    char name[DATABASE_TABLE_NAME_MAX_SIZE];
} InstructionInspectTable;

typedef struct {
    InstructionHeader instruction_header;
    TableDetails table;
} InstructionCreateTable;

typedef enum {
    INSTRUCTION_MODIFY_TABLE_TYPE_ADD_COLUMN,
    INSTRUCTION_MODIFY_TABLE_TYPE_MODIFY_COLUMN,
    INSTRUCTION_MODIFY_TABLE_TYPE_REMOVE_COLUMN
} InstructionModifyTableType;

typedef struct {
    InstructionHeader instruction_header;
    InstructionModifyTableType type;
    union {
        struct {
            int table_uid;
            TableColumnDetails* new_column_details;
        } AddColumn;
        struct {
            // todo
        } ModifyColumn;
        struct {
            int table_uid;
            int column_uid;
        } RemoveColumn;
    };
} InstructionModifyTable;

typedef struct {
    InstructionHeader instruction_header;
    int table_uid;
} InstructionDeleteTable;

typedef struct {
    InstructionHeader instruction_header;
    char new_table_name[DATABASE_TABLE_NAME_MAX_SIZE];
    int source_table_uid;
    // todo: other options to go here in the future
} InstructionCopyTable;

typedef enum {
    INSTRUCTION_SELECT_DATA_TYPE_ALL,
    INSTRUCTION_SELECT_DATA_TYPE_DISTINCT
} InstructionSelectDataType;

typedef struct {
    InstructionHeader instruction_header;
    InstructionSelectDataType type; // Defaults to ALL
    ColumnsReference* fetch_columns;
    ColumnsReference* store_columns;
    int fetch_table_uid;
    Expression* where_expression; // Must return bool
    Expression* grouping_expression; // Must return numeric
} InstructionSelectData;

typedef enum {
    INSTRUCTION_INSERT_DATA_TYPE_INSERT,
    INSTRUCTION_INSERT_DATA_TYPE_FETCH
} InstructionInsertDataType;

typedef struct {
    InstructionHeader instruction_header;
    int target_table_uid;
    InstructionInsertDataType type;
    union {
        struct {
            ColumnsReference* target_columns;
            ExpressionList* values;
        } insert_data;
        struct {
            InstructionSelectData *selection;
        } fetch_data;
    };
} InstructionInsertData;

typedef struct {
    InstructionHeader instruction_header;
    // todo: check when implementing if this is actually correct as I'm doubtful
    int target_table_uid;
    InstructionInsertDataType type;
    union {
        struct {
            ColumnsReference* target_columns;
            ExpressionList* values;
        } update_data;
        struct {
            InstructionSelectData *selection;
        } fetch_data;
    };
} InstructionUpdateData;

typedef struct {
    InstructionHeader instruction_header;
    int table_uid;
    Expression condition;
} InstructionDeleteData;

typedef union {
    InstructionHeader header;
    InstructionInspectTable inspect_table;
    InstructionCreateTable create_table;
    InstructionModifyTable modify_table;
    InstructionDeleteTable delete_table;
    InstructionCopyTable copy_table;
    InstructionSelectData select_data;
    InstructionInsertData insert_data;
    InstructionUpdateData update_data;
    InstructionDeleteData delete_data;
} Instruction;

#endif //SAPPHIRE_DB_IR_H
