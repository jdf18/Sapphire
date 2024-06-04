#ifndef SAPPHIRE_TABLE_H
#define SAPPHIRE_TABLE_H

#include "pager.h"

#include <stdint.h>

typedef struct {
    Pager* pager;
    uint32_t root_page_num;
} Table;

Table* db_open(const char* filename);
void db_close(Table* table);

#endif //SAPPHIRE_TABLE_H
