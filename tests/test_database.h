#ifndef SAPPHIRE_TEST_DATABASE_H
#define SAPPHIRE_TEST_DATABASE_H


#define MAX_INPUT_SIZE 1024

#include "statements.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char* argv[]);

#include <stdint.h>

#define COLUMN_USERNAME_SIZE 31
#define COLUMN_EMAIL_SIZE 63

typedef struct {
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
} MyValue;

typedef struct {
    uint32_t id;
    MyValue value;
} MyRow;

#endif //SAPPHIRE_TEST_DATABASE_H
