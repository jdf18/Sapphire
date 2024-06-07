#ifndef SAPPHIRE_BTREE_H
#define SAPPHIRE_BTREE_H

#include "definitions.h"

#include <stdint.h>

typedef uint8_t NodeType;
enum {
    NODE_INTERNAL = 0,
    NODE_LEAF = 1
};


// NodeHeader struct, sizes, and offsets
typedef struct {
    NodeType node_type;
    uint8_t is_root;
    uint8_t parent_pointer;
} NodeHeader;

// ((NodeHeader*)0)->node_type is used because -> needs to be used on an instance.
//   ((NodeHeader*)0) represents a NodeHeader at address 0

#define NODE_TYPE_SIZE sizeof(((NodeHeader*)0)->node_type)
#define NODE_TYPE_OFFSET offsetof(NodeHeader, node_type)

#define NODE_IS_ROOT_SIZE sizeof(((NodeHeader*)0)->is_root)
#define NODE_IS_ROOT_OFFSET offsetof(NodeHeader, is_root)

#define NODE_PARENT_POINTER_SIZE sizeof(((NodeHeader*)0)->parent_pointer)
#define NODE_PARENT_POINTER_OFFSET offsetof(NodeHeader, parent_pointer)

#define NODE_HEADER_SIZE sizeof(NodeHeader)
#define NODE_HEADER_END NODE_HEADER_SIZE


// LeafNodeHeader struct, sizes, and offsets
typedef struct {
    uint32_t num_cells;
} LeafNodeHeader;

#define LEAF_NODE_NUM_CELLS_SIZE sizeof(((LeafNodeHeader*)0)->num_cells)
#define LEAF_NODE_NUM_CELLS_OFFSET (NODE_HEADER_SIZE + offsetof(LeafNodeHeader, num_cells))

#define LEAF_NODE_HEADER_SIZE LEAF_NODE_NUM_CELLS_SIZE
#define LEAF_NODE_HEADER_END (NODE_HEADER_SIZE + LEAF_NODE_HEADER_SIZE)

// LeafNodeCell sizes, and offsets

// Cant use a struct since the size is not known at compile time so will just use void* instead
//typedef struct {
//    uint32_t key;
//    uint8_t value[];
//} LeafNodeCell;

#define LEAF_NODE_BODY_SIZE (PAGE_SIZE - LEAF_NODE_HEADER_END)

#define LEAF_NODE_KEY_SIZE sizeof(uint32_t)
#define LEAF_NODE_KEY_EL_OFFSET 0

#define LEAF_NODE_VALUE_SIZE(table) (table->ROW_SIZE)
#define LEAF_NODE_VALUE_EL_OFFSET (LEAF_NODE_KEY_EL_OFFSET + LEAF_NODE_KEY_SIZE)

#define LEAF_NODE_CELL_SIZE(table) (LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE(table))

#define LEAF_NODE_MAX_CELLS(table) (LEAF_NODE_BODY_SIZE / LEAF_NODE_CELL_SIZE(table))

// LeafNode struct, sizes, and offsets
typedef struct {
    NodeHeader node_header;
    LeafNodeHeader leaf_node_header;
    uint8_t cells[LEAF_NODE_BODY_SIZE];
} LeafNode;

typedef struct {
    NodeType node_type;
    union {
        LeafNode* leaf_node;
    };
} Node;

#endif //SAPPHIRE_BTREE_H
