#include <cstdio>

#include "differentiator.h"

int main(int argc, char *argv[]) {
  Differentiator Diff = {};

  treeInitialize    (&Diff.diffTree);
  treeInfoInitialize(&Diff.diffTree, __FILE__, __PRETTY_FUNCTION__, __LINE__);
  parseConsole      (argc, argv, &Diff.diffTree);
  binaryTreeSetInfo (&Diff.diffTree);

  Diff.diffTree.root->data.type                      = NUMERICAL_NODE;
  Diff.diffTree.root->data.nodeValue.value           = 228;

  DIFF_DUMP_(&Diff.diffTree);

  nodeLink(&Diff.diffTree, Diff.diffTree.root, LEFT);
  Diff.diffTree.root->left->data.type                = OPERATION_NODE;
  Diff.diffTree.root->left->data.nodeValue.op        = {.name = SQRT, .symbol = (char *)"sqrt", .priority = 2};

  DIFF_DUMP_(&Diff.diffTree);

  nodeLink(&Diff.diffTree, Diff.diffTree.root, RIGHT);
  Diff.diffTree.root->right->data.type               = VARIABLE_NODE;
  Diff.diffTree.root->right->data.nodeValue.varIndex = 'x';

  DIFF_DUMP_(&Diff.diffTree);

  DIFF_SAVE_DUMP_IMAGE(&Diff.diffTree);

  return 0;
}