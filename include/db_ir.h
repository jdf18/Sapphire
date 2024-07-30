#ifndef SAPPHIRE_DB_IR_H
#define SAPPHIRE_DB_IR_H

typedef struct {
} InstructionCreateTable;

typedef struct {
} InstructionModifyTable;

typedef struct {
} InstructionDeleteTable;

typedef struct {
} InstructionSelectData;

typedef struct {
} InstructionInsertData;

typedef struct {
} InstructionUpdateData;

typedef struct {
} InstructionDeleteData;


typedef enum {
    // todo: instruction for viewing table info
    INSTRUCTION_CREATE_TABLE,
    INSTRUCTION_MODIFY_TABLE,
    INSTRUCTION_DELETE_TABLE,
    INSTRUCTION_SELECT_DATA,
    INSTRUCTION_INSERT_DATA,
    INSTRUCTION_UPDATE_DATA,
    INSTRUCTION_DELETE_DATA
} InstructionType;

typedef struct {
    InstructionType instruction_type;
    union {
        InstructionCreateTable create_table;
        InstructionModifyTable modify_table;
        InstructionDeleteTable delete_table;
        InstructionSelectData select_data;
        InstructionInsertData insert_data;
        InstructionUpdateData update_data;
        InstructionDeleteData delete_data;
    };
} Instruction;

#endif //SAPPHIRE_DB_IR_H
