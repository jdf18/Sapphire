#ifndef SAPPHIRE_SQL_COMPILER_H
#define SAPPHIRE_SQL_COMPILER_H

#include "vm_ir.h"

typedef enum {
    SQL_COMPILATION_SUCCESS,
    SQL_COMPILATION_MISSING_KEYWORD,
    SQL_COMPILATION_REMAINING_TOKENS,
    SQL_COMPILATION_NO_VALID_PATHS,
    SQL_COMPILATION_INVALID_TOKEN
} SQLCompilationResult;

SQLCompilationResult compile_sql_statement(const char* input, Instruction* instruction);

#endif //SAPPHIRE_SQL_COMPILER_H
