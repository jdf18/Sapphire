#ifndef SAPPHIRE_DATABASE_H
#define SAPPHIRE_DATABASE_H

#include "../src/table.h"
#include "../src/file.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC_STRING "DBJF"

#define DB_HEADER_MAGIC_BYTES_OFFSET 0
#define DB_HEADER_VERSION_BYTES_OFFSET 4
#define DB_HEADER_PAGE_SIZE_BYTES_OFFSET 8
#define DB_HEADER_NUM_TABLES_BYTES_OFFSET 12

#define DB_TABLES_BYTES_OFFSET 16
#define DB_TABLES_LENGTH_OFFSET 0
#define DB_TABLES_ROOT_PAGE_OFFSET 4
#define DB_TABLES_IDENTIFIER_OFFSET 8
#define DB_TABLES_NUM_COLUMNS_OFFSET 16
#define DB_TABLES_COLUMNS_OFFSET 20

#define DB_COLUMN_TYPE_OFFSET 0
#define DB_COLUMN_CONSTRAINT_OFFSET 1
#define DB_COLUMN_IDENTIFIER_OFFSET 2
#define DB_COLUMN_SIZE 10

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
