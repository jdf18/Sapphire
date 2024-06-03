#include "pager.h"

#include "definitions.h"
#include "btree.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SINGLE_INDENT "    "

// Memory allocation and freeing
Pager* pager_open(const char* filename) {
    int file_descriptor = 0; // TODO: Figure out how to read/write to files
    int file_length = 0; // TODO: Implement

    if (file_length % PAGE_SIZE != 0) {
        printf("Database file is not a whole number of pages. Corrupt.\n");
        exit(EXIT_FAILURE);
    }

    Pager* pager = malloc(sizeof(Pager));
    pager->file_descriptor = file_descriptor;
    pager->file_length = file_length;
    pager->num_pages = file_length / PAGE_SIZE;

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }

    return pager;
};
// Pager freeing is handled inside db_close (free Table)



void pager_fetch_page(Pager* pager, uint32_t page_num) {
    Page* page = malloc(PAGE_SIZE);
    uint32_t num_pages = pager->file_length / PAGE_SIZE;

    if (page_num < num_pages) {
        // Existing page so read bytes of file into Page*
    } else {
        // Empty page is being created so not required to read from file
        pager->num_pages = page_num + 1;
    }
    pager->pages[page_num] = page;
}

// Returns a pointer to the specified page
Page* get_page(Pager* pager, uint32_t page_num) {
    if (page_num >= TABLE_MAX_PAGES) {
        printf("Tried to fetch a page number out of bounds. %d > %d.\n",
               page_num, TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }

    if (pager->pages[page_num] == NULL) {
        // Cache miss, allocate memory and load from the file before returning pointer
        pager_fetch_page(pager, page_num);
    }

    return pager->pages[page_num];
};


// Returns the highest key that can be indexed inside the node
uint32_t get_node_max_key(Pager* pager, Node* node) {
    if (node->node_type == NODE_LEAF) {
        uint32_t last_cell = node->leaf_node->leaf_node_header.num_cells - 1;
        return node->leaf_node->cells[last_cell].key;
    }
    // TODO: Not implemented get_node_max_key for internal nodes (rightmost child, call recursively)
    printf("Not implemented get_node_max_key for internal nodes.\n");
    exit(EXIT_FAILURE);
};

void indent(uint32_t level) {
    for (uint32_t i = 0; i < level; i++) {
        printf(SINGLE_INDENT);
    }
}

// Prints a representation of the tree
void print_tree(Pager* pager, uint32_t page_num, uint32_t indentation_level) {
    Page* page = get_page(pager, page_num);
    uint32_t num_keys;
    uint32_t child;

    switch (page->node.node_type) {
        case (NODE_LEAF):
            num_keys = page->node.leaf_node->leaf_node_header.num_cells;
            indent(indentation_level);
            printf("- leaf (size %d)\n", num_keys);
            for (uint32_t i = 0; i < num_keys; i++) {
                indent(indentation_level + 1);
                printf("- %d\n", page->node.leaf_node->cells[i].key);
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
        printf("Tried to flush a null page.\n");
        exit(EXIT_FAILURE);
    }

    //TODO: Write the page to the file
}