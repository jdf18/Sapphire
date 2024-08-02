#ifndef SAPPHIRE_DATABASE_H
#define SAPPHIRE_DATABASE_H

#include "../src/table.h"
#include "../src/file.h"
#include "../src/db_file_format.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DATABASE_COLUMN_NAME_MAX_SIZE 16
#define DATABASE_TABLE_NAME_MAX_SIZE 16

typedef struct {
    uint32_t version;
    uint32_t page_size;
    uint32_t num_tables;
    Pager* pager;
    Table** tables;
} Database;

Database* open_db(const char* filepath);
void close_db(Database* database);

#endif //SAPPHIRE_DATABASE_H
