#include "table.h"

#include "file.h"

#include <stdlib.h>

Table* db_open(const char* filename) {
    Pager* pager = pager_open(filename);

    Table* table = malloc(sizeof(Table));
    table->pager = pager;
    table->root_page_num = 0;

    // TODO: setting ROW_SIZE
    // ? Do I pass in row size as a parameter, or create a header in the database file
    table->ROW_SIZE;
    table->ROWS_PER_PAGE;

    if (pager->num_pages == 0) {
        // TODO: New db, set up page 0 as a new root node
    }

    return table;
}
void db_close(Table* table) {
    Pager* pager = table->pager;

    for (uint32_t i = 0; i < pager->num_pages; i++) {
        if (pager->pages[i] == NULL) {
            continue;
        }
        pager_flush(pager, i);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
    }

    close_file(pager->file);

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        Page* page = pager->pages[i];
        if (page) {
            free(page);
            pager->pages[i] = NULL;
        }
    }

    free(pager);
    free(table);
}
