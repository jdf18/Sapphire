#ifndef SAPPHIRE_CURSOR_H
#define SAPPHIRE_CURSOR_H

#include "table.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    Table* table;
    uint32_t row_num;
    bool end_of_table;
} Cursor;

Cursor* table_start(Table* table);
Cursor* table_end(Table* table);
uint8_t* cursor_value(Cursor* cursor);

void cursor_advance(Cursor* cursor);



#endif //SAPPHIRE_CURSOR_H
