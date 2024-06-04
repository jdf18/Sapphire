#include "btree.h"

// NodeHeader sizes, and offsets

// ((NodeHeader*)0)->node_type is used because -> needs to be used on an instance.
//   ((NodeHeader*)0) represents a NodeHeader at address 0
const uint32_t NODE_TYPE_SIZE = sizeof(((NodeHeader*)0)->node_type);
const uint32_t NODE_TYPE_OFFSET = offsetof(NodeHeader, node_type);

const uint32_t NODE_IS_ROOT_SIZE = sizeof(((NodeHeader*)0)->is_root);
const uint32_t NODE_IS_ROOT_OFFSET = offsetof(NodeHeader, is_root);

const uint32_t NODE_PARENT_POINTER_SIZE = sizeof(((NodeHeader*)0)->parent_pointer);
const uint32_t NODE_PARENT_POINTER_OFFSET = offsetof(NodeHeader, parent_pointer);

const uint32_t NODE_HEADER_SIZE = sizeof(NodeHeader);


// LeafNodeHeader sizes, and offsets

const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(((LeafNodeHeader*)0)->num_cells);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = offsetof(LeafNodeHeader, num_cells);
const uint32_t LEAF_NODE_NUM_CELLS_HEADER_OFFSET = NODE_HEADER_SIZE + offsetof(LeafNodeHeader, num_cells);

const uint32_t LEAF_NODE_HEADER_SIZE = LEAF_NODE_NUM_CELLS_SIZE;
const uint32_t LEAF_NODE_FULL_HEADER_SIZE = NODE_HEADER_SIZE + LEAF_NODE_HEADER_SIZE;


// LeafNodeCell sizes, and offsets

const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;

// TODO: ROW_SIZE is not a property of table. Somehow need to make all of this dynamic.
// ? Possibly use macros so done at compile time
// ! const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;

// ! const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;


// LeafNode sizes, and offsets

const uint32_t LEAF_NODE_CELL_SPACE = PAGE_SIZE - LEAF_NODE_FULL_HEADER_SIZE;

// ! const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_CELL_SPACE / LEAF_NODE_CELL_SIZE;
