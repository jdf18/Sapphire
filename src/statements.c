#include "../include/statements.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    TABLE_NAME_SEARCH,
    COLUMN_NAME_SEARCH,
    COLUMN_NAME_PARSE,
    VALUES_KW_SEARCH,
    VALUES_SEARCH,
    VALUES_PARSE
} InsertCompileState;

PrepareResult compile_insert(char* input, StatementInsert* statement) {
    input += 12;

    InsertCompileState state = TABLE_NAME_SEARCH;
    char* section_start = input;

    uint32_t string_offset = 0;

    while (true) {
        if (*input == '\0') {
            break;
        }

        switch (state) {
            case TABLE_NAME_SEARCH:
                if (*input == ' ') {
                    memcpy(statement->table, section_start, input-section_start);
                    state = COLUMN_NAME_SEARCH;
                }
                break;
            case COLUMN_NAME_SEARCH:
                if (*input != '(') {
                    break;
                }
                input += 1;
                section_start = input;
                state = COLUMN_NAME_PARSE;
            case COLUMN_NAME_PARSE:
                if (*input != ')') {
                    if (*input == ',') {
                        memcpy(statement->columns + string_offset, section_start, input-section_start);
                        *(char*)(statement->columns + string_offset + (uint64_t)input - section_start) = '\0';
                        string_offset += input - section_start + 1;
                        break;
                    }
                    if (*input == ' ') {
                        section_start = input + 1;
                        break;
                    }
                    break;
                }
                memcpy(statement->columns + string_offset, section_start, input-section_start);
                *(char*)(statement->columns + string_offset + (uint64_t)input - section_start) = '\0';
                input += 1;
                state = VALUES_KW_SEARCH;
            case VALUES_KW_SEARCH:
                if (*input == ' ') {
                    break;
                }
                if (strncmp(input, "values ", 6) != 0) {
                    printf("Failed to find VALUES in insert statement.\n");
                    exit(EXIT_FAILURE);
                }
                state = VALUES_SEARCH;
            case VALUES_SEARCH:
                if (*input != '(') {
                    break;
                }
                input += 1;
                string_offset = 0;
                section_start = input;
                state = VALUES_PARSE;
            case VALUES_PARSE:
                if (*input != ')') {
                    if (*input == ',') {
                        memcpy(statement->values + string_offset, section_start, input-section_start);
                        *(char*)(statement->values + string_offset + (uint64_t)input - section_start) = '\0';
                        string_offset += input - section_start + 1;
                        break;
                    }
                    if (*input == ' ') {
                        section_start = input + 1;
                        break;
                    }
                    break;
                }
                memcpy(statement->values + string_offset, section_start, input-section_start);
                *(char*)(statement->values + string_offset + (uint64_t)input - section_start) = '\0';
                input += 1;
                return PREPARE_SUCCESS;
        }

        input += 1;
    }
    return PREPARE_SYNTAX_ERROR;
}

PrepareResult compile_statement(char* input, Statement* statement) {
    if (strncmp(input, "insert into ", 12) == 0) {
        statement->type = STATEMENT_INSERT;
        return compile_insert(input, &(statement->statement_insert));
    }
    if (strncmp(input, "select ", 7) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNISED_COMMAND;
}

ExecuteResult execute_insert(StatementInsert * statement) {
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(StatementSelect * statement) {
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            return execute_insert(&(statement->statement_insert));
        case (STATEMENT_SELECT):
            return execute_select(&(statement->statement_select));
    }
    return EXECUTE_SUCCESS;
}