#include "cursor.h"

#include <stdlib.h>
#include <stdint.h>

Cursor* table_start(Table* table) {
    Cursor* cursor = malloc(sizeof(Cursor));
    cursor->table = table;
    cursor->row_num = 0;
    cursor->end_of_table = false; // TODO: Check if the number of rows in the table is 0

    return cursor;
}

Cursor* table_end(Table* table) {
    Cursor* cursor = malloc(sizeof(Cursor));
    cursor->table = table;
    cursor->row_num = 0; // TODO: Number of rows in the table
    cursor->end_of_table = true;

    return cursor;
}

uint8_t* cursor_value(Cursor* cursor) {
    uint32_t row_num = cursor->row_num;
    uint32_t page_num = row_num / cursor->table->ROWS_PER_PAGE;

    Page* page = get_page(cursor->table->pager, page_num);
    uint32_t row_offset = row_num % cursor->table->ROWS_PER_PAGE;

    if (page->node.node_type != NODE_LEAF) {
        // TODO: What do you do if its an internal node?
        printf("Node was not of type leaf.\n");
        exit(EXIT_FAILURE);
    }

    return (page->node.leaf_node->cells[row_offset].value);
}

void cursor_advance(Cursor* cursor) {
    if (cursor->end_of_table) {
        printf("Cursor at the end of table. Can not advance.\n");
        exit(EXIT_FAILURE);
    }

    cursor->row_num += 1;

    // TODO: if row_num == table no of rows, set end of table to true
}