#ifndef DIFFERENTIATOR_H_
#define DIFFERENTIATOR_H_

#include <cstdlib>
#include <cmath>

#include "binaryTree.h"

enum DiffError {
  NO_DIFF_ERRORS    =      0,
  DIFF_NULL_PTR     = 1 << 0,
  TREE_ERROR        = 1 << 1,
  NODE_NULL_PTR     = 1 << 2,
  WRONG_OPERATION   = 1 << 3,
  NO_VALUE          = 1 << 4,
  FILE_ERROR        = 1 << 5,
  DUMP_FILE_ERROR   = 1 << 6,
  BUFFER_ERROR      = 1 << 7
};

enum NodeType {
  NUMERICAL_NODE = -2,
  OPERATION_NODE = -1,
  VARIABLE_NODE  =  0
};

enum Operation {
  ADD  = 1 << 0,
  SUB  = 1 << 1,
  MUL  = 1 << 2,
  DIV  = 1 << 3,
  POW  = 1 << 4,
  SIN  = 1 << 5,
  COS  = 1 << 6,
  EXP  = 1 << 7,
  LN   = 1 << 8,
  SQRT = 1 << 9
};

struct OperationInfo {
  Operation name     =   {};
  char     *symbol   =   {};
  size_t    priority =    0;
};

union NodeData {
  double           value;
  char             varIndex;
  OperationInfo    op;
};

struct DiffNode {
  NodeType  type       = NUMERICAL_NODE;
  NodeData  nodeValue  = {.value = NAN};
};

struct Differentiator {
  binaryTree<DiffNode> diffTree    =             {};
  DiffError            errorStatus = NO_DIFF_ERRORS;
};

#endif // DIFFERENTIATOR_H_