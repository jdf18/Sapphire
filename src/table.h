#ifndef SAPPHIRE_TABLE_H
#define SAPPHIRE_TABLE_H

#include "pager.h"

#include <stdint.h>

#define BLOB_SIZE_MULTIPLIER 4

typedef struct {
    uint8_t type;
    uint8_t constraints;
    char identifier[8];
} TableSchemaColumn;

typedef struct {
    uint32_t ROW_SIZE;
    uint32_t ROWS_PER_PAGE;
    uint32_t num_columns;
    TableSchemaColumn columns[];
} TableSchema;

typedef struct {
    Pager* pager;
    uint32_t root_page_num;
    uint32_t ROWS_PER_PAGE; // TODO Make const
    uint32_t ROW_SIZE;
    TableSchema* schema;
    char identifier[8];
} Table;

Table* init_table(Pager* pager);
void free_table(Table* table);

void update_row_sizes_from_schema(Table* table);

// Allows writing of binary strings
#define B(x) S_to_binary_(#x)

static inline unsigned long long S_to_binary_(const char *s)
{
    unsigned long long i = 0;
    while (*s) {
        i <<= 1;
        i += *s++ - '0';
    }
    return i;
}

#endif //SAPPHIRE_TABLE_H
