#include <cstdlib>
#include <cmath>

#include "binaryTree.h"

#define DIFF_DUMP_(treePtr) { \
  strncpy((treePtr)->infoData->lastUsedFileName,     __FILE__,            MAX_FILE_NAME_SIZE); \
  strncpy((treePtr)->infoData->lastUsedFunctionName, __PRETTY_FUNCTION__, MAX_FILE_NAME_SIZE); \
  (treePtr)->infoData->lastUsedLine  =               __LINE__                                ; \
  diffTreeDump(treePtr);                                                                       \
}

#define DIFF_SAVE_DUMP_IMAGE(treePtr) {                                 \
  DIFF_DUMP_(treePtr);                                                  \
                                                                        \
  char *buffer = (char *)calloc(MAX_CMD_BUFFER_SIZE, sizeof(char));     \
  customWarning(buffer != NULL, BAD_BUFFER_POINTER);                    \
                                                                        \
  char *newFileName = (char *)calloc(MAX_FILE_NAME_SIZE, sizeof(char)); \
  customWarning(newFileName != NULL, BAD_FILE_NAME_POINTER);            \
                                                                        \
  strncpy(newFileName, (treePtr)->infoData->htmlDumpPath,               \
  strlen((treePtr)->infoData->htmlDumpPath) - strlen(".html"));         \
                                                                        \
  snprintf(buffer, MAX_CMD_BUFFER_SIZE, "dot -Tsvg %s -o %s.svg",       \
          (treePtr)->infoData->dumpFileName, newFileName);              \
  system(buffer);                                                       \
                                                                        \
  FREE_(buffer);                                                        \
  FREE_(newFileName);                                                   \
}

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

// FUNCTION PROTOTYPES //
DiffError diffTreeDump         (binaryTree<DiffNode> *tree);
DiffError diffTreeNodeDump     (FILE *dumpFile, node<DiffNode> *node);
DiffError diffTreeNodeDumpLink (FILE *dumpFile, node<DiffNode> *node);
// FUNCTION PROTOTYPES //