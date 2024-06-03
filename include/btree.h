#ifndef SAPPHIRE_BTREE_H
#define SAPPHIRE_BTREE_H

#include "definitions.h"

#include <stdint.h>

#define ROW_SIZE 0

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

extern const uint32_t NODE_TYPE_SIZE;
extern const uint32_t NODE_TYPE_OFFSET;

extern const uint32_t NODE_IS_ROOT_SIZE;
extern const uint32_t NODE_IS_ROOT_OFFSET;

extern const uint32_t NODE_PARENT_POINTER_SIZE;
extern const uint32_t NODE_PARENT_POINTER_OFFSET;

extern const uint32_t NODE_HEADER_SIZE;


// LeafNodeHeader struct, sizes, and offsets
typedef struct {
    uint32_t num_cells;
} LeafNodeHeader;

extern const uint32_t LEAF_NODE_NUM_CELLS_SIZE;
extern const uint32_t LEAF_NODE_NUM_CELLS_OFFSET;
extern const uint32_t LEAF_NODE_NUM_CELLS_HEADER_OFFSET;

extern const uint32_t LEAF_NODE_HEADER_SIZE;
extern const uint32_t LEAF_NODE_FULL_HEADER_SIZE;


// LeafNodeCell struct, sizes, and offsets
typedef struct {
    uint32_t key;
    uint8_t value[];
} LeafNodeCell;

extern const uint32_t LEAF_NODE_KEY_SIZE;
extern const uint32_t LEAF_NODE_KEY_OFFSET;

extern const uint32_t LEAF_NODE_VALUE_SIZE;
extern const uint32_t LEAF_NODE_VALUE_OFFSET;

extern const uint32_t LEAF_NODE_CELL_SIZE;


// LeafNode struct, sizes, and offsets
typedef struct {
    NodeHeader node_header;
    LeafNodeHeader leaf_node_header;
    LeafNodeCell cells[];
} LeafNode;

extern const uint32_t LEAF_NODE_CELL_SPACE;
extern const uint32_t LEAF_NODE_MAX_CELLS;


typedef struct {
    NodeType node_type;
    union {
        LeafNode* leaf_node;
    };
} Node;

#endif //SAPPHIRE_BTREE_H
