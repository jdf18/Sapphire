#ifndef SAPPHIRE_VM_IR_H
#define SAPPHIRE_VM_IR_H

#include <stdint.h>

typedef enum {
    INSTRUCTION_CREATE,
    INSTRUCTION_DROP,
    INSTRUCTION_SELECT,
    INSTRUCTION_INSERT,
    INSTRUCTION_UPDATE,
    INSTRUCTION_DELETE
} InstructionType;

typedef struct {
} InstructionCreate;

typedef struct {
} InstructionDrop;

typedef struct {
} InstructionSelect;

typedef struct {
} InstructionInsert;

typedef struct {
} InstructionUpdate;

typedef struct {
} InstructionDelete;

typedef struct {
    InstructionType type;
    union {
        InstructionCreate instruction_create;
        InstructionDrop instruction_drop;
        InstructionSelect instruction_select;
        InstructionInsert instruction_insert;
        InstructionUpdate instruction_update;
        InstructionDelete instruction_delete;
    };
} Instruction;

#endif //SAPPHIRE_VM_IR_H
