#ifndef DIFF_DUMP_H_
#define DIFF_DUMP_H_

#include "binaryTree.h"
#include "differentiator.h"

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

// FUNCTION PROTOTYPES //
DiffError diffTreeDump         (binaryTree<DiffNode> *tree);
DiffError diffTreeNodeDump     (FILE *dumpFile, node<DiffNode> *node);
DiffError diffTreeNodeDumpLink (FILE *dumpFile, node<DiffNode> *node);
// FUNCTION PROTOTYPES //

#endif // DIFF_DUMP_H_