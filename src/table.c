#include "table.h"
#include "file.h"

#include "../libs/Topaz/topaz.h"

#include <stdlib.h>
#include <string.h>

Table* create_table(Pager* pager) {
    Table* table = malloc(sizeof(Table));
    table->pager = pager;
    table->root_page_num = pager->num_pages;

    table->ROWS_PER_PAGE = 0;
    table->ROW_SIZE=0;

    // ! Not implemented yet
    LOG_ERROR("Not implemented yet table.c", "");
    exit(EXIT_FAILURE);
    // TODO: New db table, set up a new root node

    return table;
}

uint32_t calculate_data_type_size(TableSchemaColumn* column) {
    // Takes in a type definition and returns the size of it in bytes
    uint8_t mask = 0x80;
    uint8_t data_class = (column->type & mask);
    if (data_class == B(00000000)) {
        // INTEGER (signed/unsigned) / FLOAT / DECIMAL
        mask = B(00011111);
        uint8_t operand = column->type & mask;
        switch (operand) {
            case 0: // int8
                return 1;
            case 1: // int16
                return 2;
            case 3: // int32
                return 4;
            case 4: // int64
                return 8;
            default:
                LOG_ERROR("Invalid data type definition");
                exit(EXIT_FAILURE);
        }
    } else if (data_class == B(10000000)) {
        // STRING / BLOB
        mask = B(00111111);
        uint8_t operand = column->type & mask;
        return operand * BLOB_SIZE_MULTIPLIER;
    }
    LOG_ERROR("Did not return a size from data type definition.");
    exit(EXIT_FAILURE);
}

void update_row_sizes_from_schema(Table* table) {
    uint32_t num_columns = table->schema->num_columns;
    uint32_t row_size = 4; // Initial row size if 4 for uint32 identifier
    for (uint32_t i = 0; i < num_columns; i++) {
        row_size = row_size + calculate_data_type_size(&(table->schema->columns[i]));
    }
    table->schema->ROW_SIZE = row_size;
    table->schema->ROWS_PER_PAGE = PAGE_SIZE / row_size;
}

void free_table(Table* table) {
    free(table);
}

