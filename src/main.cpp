#include <cstdio>

#include "differentiator.h"
#include "diffDump.h"
#include "diffIO.h"
#include "DSL.h"
#include "lexer.h"
#include "recursiveDescentParser.h"

int main(int argc, char *argv[]) {
  Differentiator Diff = {};

  diffInitialize      (&Diff, argc, argv, __FILE__, __PRETTY_FUNCTION__, __LINE__);

  expressionLexer(&Diff);

  Diff.diffTree.root = getG(&Diff);

  callPrintBinaryTree(&Diff.diffTree, INFIX, stdout);

  DIFF_DUMP_(&Diff.diffTree);

  DIFF_SAVE_DUMP_IMAGE(&Diff.diffTree);

  return 0;
}
