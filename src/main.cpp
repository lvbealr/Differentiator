#include <cstdio>

#include "differentiator.h"
#include "diffDump.h"
#include "diffIO.h"
#include "DSL.h"
#include "lexer.h"

int main(int argc, char *argv[]) {
  Differentiator Diff = {};

  diffInitialize      (&Diff, argc, argv, __FILE__, __PRETTY_FUNCTION__, __LINE__);

  expressionLexer(&Diff);

  Diff.diffTree.root->left  = CONST_(5);
  DIFF_DUMP_(&Diff.diffTree);
  Diff.diffTree.root->right = CONST_(3);
  DIFF_DUMP_(&Diff.diffTree);
  Diff.diffTree.root        = OPERATION_NODE_(Diff.diffTree.root->left, Diff.diffTree.root->right, POW);
  DIFF_DUMP_(&Diff.diffTree);

  DIFF_SAVE_DUMP_IMAGE(&Diff.diffTree);

  return 0;
}
