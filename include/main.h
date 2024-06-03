#ifndef SAPPHIRE_MAIN_H
#define SAPPHIRE_MAIN_H

#define MAX_INPUT_SIZE 1024

#include "statements.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNISED
} MetaCommandResult;

void print_prompt();

MetaCommandResult execute_meta_command(char * input);

int main(int argc, char* argv[]);


#endif //SAPPHIRE_MAIN_H
