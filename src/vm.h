#ifndef SAPPHIRE_VM_H
#define SAPPHIRE_VM_H

#include "db_ir.h"

typedef enum {
    EXECUTION_SUCCESS,
    EXECUTION_FAILURE
} ExecutionResult;

ExecutionResult executeInstruction(Instruction* instruction);

#endif //SAPPHIRE_VM_H
