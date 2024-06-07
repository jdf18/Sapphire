#include "test_database.h"

#include "table.h"

#include <stdio.h>

#define FILEPATH "C:\\Users\\Jacob\\CLionProjects\\Sapphire\\test.db"

void print_prompt() { printf("db>"); }

MetaCommandResult execute_meta_command(char * input) {
    if (strcmp(input, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNISED;
    }
}

int main(int argc, char* argv[]) {
    setbuf(stdout, 0);

    Table* table = db_open(FILEPATH);

    // * Temporary code until table class is working
    table->ROW_SIZE = sizeof(MyValue);
    table->pager->ROW_SIZE = sizeof(MyValue);
    // ? Should this be equal to LEAF_NODE_MAX_CELLS
    table->ROWS_PER_PAGE = PAGE_SIZE / table->ROW_SIZE;

    char input[MAX_INPUT_SIZE];

    while (true) {
        // Prompt and receive user input
        print_prompt();
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = '\0';

        // Handle meta-commands (and continue)
        if (input[0] == '.') {
            switch (execute_meta_command(input)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNISED):
                    printf("Unrecognised command '%s'.\n", input);
                    continue;
            }
        }

        // Compile command
        Statement statement;
        switch (compile_statement(input, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax error: could not parse the statement.\n");
                continue;
            case (PREPARE_NEGATIVE_ID):
                printf("Syntax error: ID should be positive.\n");
                continue;
            case (PREPARE_UNRECOGNISED_COMMAND):
                printf("Unrecognised command at the start of '%s'.\n", input);
                continue;
        }

        execute_statement(&statement);
        printf("Executed.\n");
    }

    db_close(table);

    return 0;
}


void serialise_row();

MyRow deserialise_row();