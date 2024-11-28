#ifndef DIFFERENTIATOR_H_
#define DIFFERENTIATOR_H_

#include <cstdlib>
#include <cmath>

#include "binaryTree.h"

enum diffError {
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

enum nodeType {
  NUMERICAL_NODE = -2,
  OPERATION_NODE = -1,
  VARIABLE_NODE  =  0
};

#define OPERATOR(NAME, ...) NAME,
enum Operation {
  #include "diffOperations.def"
};
#undef OPERATOR

#define OPERATOR(...) 1 +
const size_t OPERATIONS_NUMBER =
                               #include "diffOperations.def"
                               0;
#undef OPERATOR

struct operationInfo {
  Operation   name     =   {};
  const char *symbol   =   {};
  size_t      priority =    0;
};

union nodeData {
  double           value;
  char             varIndex;
  Operation        op;
};

struct diffNode {
  nodeType  type       = NUMERICAL_NODE;
  nodeData  nodeValue  = {.value = NAN};
};

struct variableTable {
  char  *variableName  = {};
  double variableValue = NAN;
};

struct Differentiator {
  binaryTree<diffNode> diffTree    =             {};
  variableTable       *variables   =             {};
  diffError            errorStatus = NO_DIFF_ERRORS;
};

// FUNCTION PROTOTYPES //
diffError diffInitialize(Differentiator *diff);
diffError diffDestruct  (Differentiator *diff);
diffError diffVerify    (Differentiator *diff);

diffError evalTree      (Differentiator *diff, double *treeValue);
// FUNCTION PROROTYPES //

#endif // DIFFERENTIATOR_H_