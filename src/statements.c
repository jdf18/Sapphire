#include "../include/statements.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PrepareResult compile_statement(char* input, Statement* statement) {
    if (strncmp(input, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;

        return PREPARE_SUCCESS;
    }
    if (strncmp(input, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNISED_COMMAND;
}

ExecuteResult execute_insert(Statement* statement) {

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement* statement) {
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            return execute_insert(statement);
        case (STATEMENT_SELECT):
            return execute_select(statement);
    }
    return EXECUTE_SUCCESS;
}