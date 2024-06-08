#include "test_database.h"

#include "database.h"

#include <stdio.h>

#define FILEPATH "C:\\Users\\Jacob\\CLionProjects\\Sapphire\\test.db"

void print_prompt() { printf("db>"); }

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNISED
} MetaCommandResult;

MetaCommandResult execute_meta_command(char * input) {
    if (strcmp(input, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNISED;
    }
}

void REPL(char* input) {
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
    }
}

int main(int argc, char* argv[]) {
    setbuf(stdout, 0);

    Database* database = open_db(FILEPATH);

    Table* table = database->tables[0];

    // Check that the row size matches that of the struct we will use to store it.
    if (table->schema->ROW_SIZE != sizeof(MyRow)) {
        printf("Row sizes do not match between database and programs struct. (%d:%llu)", table->schema->ROW_SIZE, sizeof(MyRow));
        exit(EXIT_FAILURE);
    }

    // Set up an input buffer for the REPL
    char input[MAX_INPUT_SIZE];

    // Enter into the Read-Evaluate-Print-Loop
    REPL(input);

    // Close the database and free the rest of memory being used
    close_db(database);

    return 0;
}