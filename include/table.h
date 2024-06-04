#ifndef SAPPHIRE_TABLE_H
#define SAPPHIRE_TABLE_H

#include "pager.h"

#include <stdint.h>

typedef struct {
    Pager* pager;
    uint32_t root_page_num;
    const uint32_t ROWS_PER_PAGE;
    const uint32_t ROW_SIZE;
} Table;

Table* db_open(const char* filename);
void db_close(Table* table);

#endif //SAPPHIRE_TABLE_H
