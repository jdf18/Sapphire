#ifndef SAPPHIRE_DATABASE_H
#define SAPPHIRE_DATABASE_H

#include "table.h"
#include "file.h"
#include "db_file_format.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t version;
    uint32_t page_size;
    uint32_t num_tables;
    Pager* pager;
    Table** tables;
} Database;

Database* create_db(const char* filepath);
Database* open_db(const char* filepath);
void close_db(Database* database);

#endif //SAPPHIRE_DATABASE_H
