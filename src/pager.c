#include "pager.h"

#include "definitions.h"
#include "btree.h"
#include "file.h"

#include "../libs/Topaz/topaz.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SINGLE_INDENT "    "

// Memory allocation and freeing
Pager* pager_open(const char* filename) {
    FILE* file = open_file(filename);
    long file_length = get_file_length(file);

    if (file_length % PAGE_SIZE != 0) {
        LOG_ERROR("Database file is not a whole number of pages. (%ld Bytes)\n", file_length);
        exit(EXIT_FAILURE);
    }

    Pager* pager = malloc(sizeof(Pager));
    pager->file = file;
    pager->file_length = file_length;
    pager->num_pages = file_length / PAGE_SIZE;

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }

    return pager;
}
// Pager freeing is handled inside db_close (free Table)



void pager_fetch_page(Pager* pager, uint32_t page_num) {
    Page* page = malloc(PAGE_SIZE);
    uint32_t num_pages = pager->file_length / PAGE_SIZE;

    if (page_num < num_pages) {
        // Existing page so read bytes of file into Page*
        read_file_into_memory(pager->file, page, PAGE_SIZE, PAGE_SIZE*page_num);
    } else {
        // Empty page is being created so not required to read from file
        pager->num_pages = page_num + 1;
    }
    pager->pages[page_num] = page;
}

// Returns a pointer to the specified page
Page* get_page(Pager* pager, uint32_t page_num) {
    if (page_num >= TABLE_MAX_PAGES) {
        LOG_ERROR("Tried to fetch a page number out of bounds. %d > %d.\n",
               page_num, TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }

    if (pager->pages[page_num] == NULL) {
        // Cache miss, allocate memory and load from the file before returning pointer
        pager_fetch_page(pager, page_num);
    }

    return pager->pages[page_num];
}


// Returns the highest key that can be indexed inside the node
uint32_t get_node_max_key(Pager* pager, Node* node) {
    if (node->node_type == NODE_LEAF) {
        uint32_t last_cell = node->leaf_node->leaf_node_header.num_cells - 1;
        return *(node->leaf_node->cells + LEAF_NODE_CELL_SIZE(pager));
    }
    // TODO: Not implemented get_node_max_key for internal nodes (rightmost child, call recursively)
    LOG_ERROR("Not implemented get_node_max_key for internal nodes.\n");
    exit(EXIT_FAILURE);
}

void indent(uint32_t level) {
    for (uint32_t i = 0; i < level; i++) {
        printf(SINGLE_INDENT);
    }
}

// Prints a representation of the tree
void print_tree(Pager* pager, uint32_t page_num, uint32_t indentation_level) {
    Page* page = get_page(pager, page_num);
    uint32_t num_keys;

    switch (page->node.node_type) {
        case (NODE_LEAF):
            num_keys = page->node.leaf_node->leaf_node_header.num_cells;
            indent(indentation_level);
            printf("- leaf (size %d)\n", num_keys);
            for (uint32_t i = 0; i < num_keys; i++) {
                indent(indentation_level + 1);
                printf("- %d\n", *(page->node.leaf_node->cells + i*LEAF_NODE_CELL_SIZE(pager) + LEAF_NODE_KEY_EL_OFFSET));
            }
            break;
        case (NODE_INTERNAL):
            //TODO: Need to handle this case once i actually figure out internal nodes
            break;
    }
}

// Writes the contents of the page to the file
void pager_flush(Pager* pager, uint32_t page_num) {
    if (pager->pages[page_num] == NULL) {
        LOG_ERROR("Tried to flush a null page.\n");
        exit(EXIT_FAILURE);
    }

    memory_write_to_file(pager->file, pager->pages[page_num], PAGE_SIZE, PAGE_SIZE*page_num);
}