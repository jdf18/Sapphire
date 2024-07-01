#ifndef SAPPHIRE_VM_EXECUTOR_H
#define SAPPHIRE_VM_EXECUTOR_H

#include "vm_ir.h"

typedef enum {
    EXECUTION_SUCCESS
} ExecutionResult;

ExecutionResult executeInstruction(Instruction* instruction);

#endif //SAPPHIRE_VM_EXECUTOR_H
