#include <cstdlib>
#include <cmath>

#include "binaryTree.h"

enum DiffError {
  NO_DIFF_ERRORS    =      0,
  DIFF_NULL_PTRR    = 1 << 0,
  TREE_ERROR        = 1 << 1,
  NODE_NULL_PTR     = 1 << 2,
  WRONG_OPERATION   = 1 << 3,
  NO_VALUE          = 1 << 4,
  FILE_ERROR        = 1 << 5,
};

enum NodeType {
  NUMERICAL_NODE = 1 << 0,
  OPERATION_NODE = 1 << 1,
  VARIABLE_NODE  = 1 << 2
};

enum Operation {
  ADD  = 0,
  SUB  = 1,
  MUL  = 2,
  DIV  = 3,
  POW  = 4,
  SIN  = 5,
  COS  = 6,
  EXP  = 7,
  LN   = 8,
  SQRT = 9
};

struct OperationInfo {
  Operation name     =   {};
  char     *symbol   = NULL;
  size_t    priority =    0;
};

union NodeData {
  double           value;
  size_t           varIndex;
  Operation        op;
};

struct DiffNode {
  NodeType  type       = NUMERICAL_NODE;
  NodeData  nodeValue  = {.value = NAN};
};

struct Differentiator {
  binaryTree<DiffNode> diffTree    =             {};
  DiffError            errorStatus = NO_DIFF_ERRORS;
};
