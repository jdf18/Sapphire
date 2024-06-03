#ifndef SAPPHIRE_BTREE_H
#define SAPPHIRE_BTREE_H

#include <stdint.h>

#define ROW_SIZE 0
#define PAGE_SIZE 4096

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
const uint32_t NODE_TYPE_SIZE = sizeof(((NodeHeader*)0)->node_type);
const uint32_t NODE_TYPE_OFFSET = offsetof(NodeHeader, node_type);

const uint32_t NODE_IS_ROOT_SIZE = sizeof(((NodeHeader*)0)->is_root);
const uint32_t NODE_IS_ROOT_OFFSET = offsetof(NodeHeader, is_root);

const uint32_t NODE_PARENT_POINTER_SIZE = sizeof(((NodeHeader*)0)->parent_pointer);
const uint32_t NODE_PARENT_POINTER_OFFSET = offsetof(NodeHeader, parent_pointer);

const uint32_t NODE_HEADER_SIZE = sizeof(NodeHeader);


// LeafNodeHeader struct, sizes, and offsets
typedef struct {
    uint32_t num_cells;
} LeafNodeHeader;

const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(((LeafNodeHeader*)0)->num_cells);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = offsetof(LeafNodeHeader, num_cells);
const uint32_t LEAF_NODE_NUM_CELLS_HEADER_OFFSET = NODE_HEADER_SIZE + offsetof(LeafNodeHeader, num_cells);

const uint32_t LEAF_NODE_HEADER_SIZE = LEAF_NODE_NUM_CELLS_SIZE;
const uint32_t LEAF_NODE_FULL_HEADER_SIZE = NODE_HEADER_SIZE + LEAF_NODE_HEADER_SIZE;


// LeafNodeCell struct, sizes, and offsets
typedef struct {
    uint32_t key;
    uint8_t value[];
} LeafNodeCell;

const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;

const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;

const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;


// LeafNode struct, sizes, and offsets
typedef struct {
    NodeHeader node_header;
    LeafNodeHeader leaf_node_header;
    LeafNodeCell cells[];
} LeafNode;

const uint32_t LEAF_NODE_CELL_SPACE = PAGE_SIZE - LEAF_NODE_FULL_HEADER_SIZE;

const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_CELL_SPACE / LEAF_NODE_CELL_SIZE;

#endif //SAPPHIRE_BTREE_H
