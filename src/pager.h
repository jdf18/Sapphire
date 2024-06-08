#ifndef SAPPHIRE_PAGER_H
#define SAPPHIRE_PAGER_H

#include "btree.h"

#include <stdio.h>
#include <stdint.h>

#define TABLE_MAX_PAGES 400

typedef struct {
    union {
        Node node;
    };
} Page;

// TODO: Track which pages have been modified and only flush changed pages.

typedef struct {
    FILE* file;
    uint32_t file_length;
    uint32_t num_pages;
    uint32_t ROW_SIZE;
    Page* pages[TABLE_MAX_PAGES];
} Pager;


// Memory allocation
Pager* pager_open(const char* filename);

// Returns a pointer to the specified page
Page* get_page(Pager* pager, uint32_t page_num);

// Returns the highest key that can be indexed inside the node
uint32_t get_node_max_key(Pager* pager, Node* node);

// Prints a representation of the tree
void print_tree(Pager* pager, uint32_t page_num, uint32_t indentation_level);

// Writes the contents of the page to the file
void pager_flush(Pager* pager, uint32_t page_num);

#endif //SAPPHIRE_PAGER_H