#ifndef SAPPHIRE_STATEMENTS_H
#define SAPPHIRE_STATEMENTS_H

#include <stdint.h>

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_NEGATIVE_ID,
    PREPARE_UNRECOGNISED_COMMAND
} PrepareResult;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    char table[50];
    char columns[50];
    char values[50];
} StatementInsert;

typedef struct {

} StatementSelect;

typedef struct {
    StatementType type;
    union {
        StatementInsert statement_insert;
        StatementSelect statement_select;
    };
} Statement;

PrepareResult compile_statement(char * input, Statement * statement);

ExecuteResult execute_statement(Statement* statement);

#endif //SAPPHIRE_STATEMENTS_H
