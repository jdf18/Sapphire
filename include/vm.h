#ifndef SAPPHIRE_VM_H
#define SAPPHIRE_VM_H

#include <stdlib.h>
#include "db_ir.h"

typedef enum {
    EXECUTION_SUCCESS,
    EXECUTION_FAILURE
} ExecutionResult;

typedef struct {
    ExecutionResult execution_result;
    InstructionType instruction_type;
} ExecutionInformation;

typedef struct {
    ExecutionInformation execution_information;
    size_t num_results;
    TableDetails* table_details[];
} InspectTableResult;

typedef struct {
    ExecutionInformation execution_information;
    bool successful;
    // Returns new table details as this may be different to the ones requested in some cases
    TableDetails new_table_details;
} CreateTableResult;

typedef struct {
    ExecutionInformation execution_information;
    bool successful;
    TableDetails new_table_details;
} ModifyTableResult;

typedef struct {
    ExecutionInformation execution_information;
    bool successful;
} DeleteTableResult;

typedef struct {
    ExecutionInformation execution_information;
    bool successful;
} CopyTableResult;



// Union of all possible result types that could be returned.
// Only properties which must be valid is execution_information.
// This can be used to confirm the type of result which is returned.
typedef union {
    ExecutionInformation execution_information;
    InspectTableResult inspect_table;
    CreateTableResult create_table;
    ModifyTableResult modify_table;
    DeleteTableResult delete_table;
    CopyTableResult copy_table;
} ResultUnion;

InspectTableResult* executeInspectTable(void* vm, InstructionInspectTable* instruction);
CreateTableResult* executeCreateTable(void* vm, InstructionCreateTable* instruction);
ModifyTableResult* executeModifyTable(void* vm, InstructionModifyTable* instruction);
DeleteTableResult* executeDeleteTable(void* vm, InstructionDeleteTable* instruction);
CopyTableResult* executeCopyTable(void* vm, InstructionCopyTable* instruction);
ResultUnion* executeInstruction(void* vm, Instruction* instruction);

#endif //SAPPHIRE_VM_H
