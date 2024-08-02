#include "vm.h"

#include "../libs/Topaz/topaz.h"

typedef struct {
    // todo
} VMState;

ResultUnion* executeInstruction(void* vm, Instruction* instruction) {
    switch (instruction->header.instruction_type) {
        case INSTRUCTION_INSPECT_TABLE:
            return (ResultUnion*) executeInspectTable(vm, (InstructionInspectTable*)instruction);
        case INSTRUCTION_CREATE_TABLE:
            return (ResultUnion*) executeCreateTable(vm, (InstructionCreateTable*)instruction);
        case INSTRUCTION_MODIFY_TABLE:
            return (ResultUnion*) executeModifyTable(vm, (InstructionModifyTable*)instruction);
        case INSTRUCTION_DELETE_TABLE:
            return (ResultUnion*) executeDeleteTable(vm, (InstructionDeleteTable*)instruction);
        case INSTRUCTION_COPY_TABLE:
            return (ResultUnion*) executeCopyTable(vm, (InstructionCopyTable*)instruction);
        case INSTRUCTION_SELECT_DATA:
        case INSTRUCTION_INSERT_DATA:
        case INSTRUCTION_UPDATE_DATA:
        case INSTRUCTION_DELETE_DATA:
            LOG_ERROR("todo implementation in the VM");
            // todo
            break;
        case INSTRUCTION_UNKNOWN:
            LOG_ERROR("DB VM can not execute an instruction with an unknown type.");
            break;
    }
    // Each successful code path above should have returned so create a result to return showing the failure.
    LOG_ERROR("An unknown error occurred inside the DB VM");
    ExecutionInformation* execution_information = malloc(sizeof(ExecutionInformation));
    execution_information->execution_result = EXECUTION_FAILURE;
    execution_information->instruction_type = INSTRUCTION_UNKNOWN;
    return (ResultUnion*) execution_information;
}

void* init_new_vm() {
    LOG_ERROR("todo implementation in the VM");
    // todo
    return (void*)NULL;
}

InspectTableResult* executeInspectTable(void* vm, InstructionInspectTable* instruction) {
    VMState* vm_state = (VMState*)vm;
    LOG_ERROR("todo implementation in the VM");
    // todo
    return NULL;
}

CreateTableResult* executeCreateTable(void* vm, InstructionCreateTable* instruction) {
    VMState* vm_state = (VMState*)vm;
    LOG_ERROR("todo implementation in the VM");
    // todo
    return NULL;
}

ModifyTableResult* executeModifyTable(void* vm, InstructionModifyTable* instruction) {
    VMState* vm_state = (VMState*)vm;
    LOG_ERROR("todo implementation in the VM");
    // todo
    return NULL;
}

DeleteTableResult* executeDeleteTable(void* vm, InstructionDeleteTable* instruction) {
    VMState* vm_state = (VMState*)vm;
    LOG_ERROR("todo implementation in the VM");
    // todo
    return NULL;
}

CopyTableResult* executeCopyTable(void* vm, InstructionCopyTable* instruction) {
    VMState* vm_state = (VMState*)vm;
    LOG_ERROR("todo implementation in the VM");
    // todo
    return NULL;
}

void free_vm(void* vm) {
    LOG_ERROR("todo implementation in the VM");
    //todo
    free(vm);
}