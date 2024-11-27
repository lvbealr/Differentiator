#include <cstdio>

#include "differentiator.h"
#include "diffDump.h"
#include "diffIO.h"

int main(int argc, char *argv[]) {
  Differentiator Diff = {};

  treeInitialize    (&Diff.diffTree);
  treeInfoInitialize(&Diff.diffTree, __FILE__, __PRETTY_FUNCTION__, __LINE__);
  parseConsole      (argc, argv, &Diff.diffTree);
  binaryTreeSetInfo (&Diff.diffTree);

  Diff.diffTree.root->data.type                      = OPERATION_NODE;;
  Diff.diffTree.root->data.nodeValue.op              = {.name = ADD, .symbol = (char *)"+", .priority = 2};

  DIFF_DUMP_(&Diff.diffTree);

  nodeLink(&Diff.diffTree, Diff.diffTree.root, LEFT);
  Diff.diffTree.root->left->data.type                = NUMERICAL_NODE;
  Diff.diffTree.root->left->data.nodeValue.value     = 100;

  DIFF_DUMP_(&Diff.diffTree);

  nodeLink(&Diff.diffTree, Diff.diffTree.root, RIGHT);
  Diff.diffTree.root->right->data.type               = VARIABLE_NODE;
  Diff.diffTree.root->right->data.nodeValue.varIndex = 'x';

  DIFF_SAVE_DUMP_IMAGE(&Diff.diffTree);

  FILE *io = fopen("IO/output.txt", "w");

  callPrintBinaryTree(&Diff.diffTree, INFIX, io);

  return 0;
}