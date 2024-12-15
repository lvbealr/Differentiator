#include <cstdio>

#include "differentiator.h"
#include "diffDump.h"
#include "diffIO.h"
#include "DSL.h"
#include "lexer.h"
#include "recursiveDescentParser.h"
#include "simplifyTree.h"

int main(int argc, char *argv[]) {
  Differentiator Diff = {};

  diffInitialize(&Diff, argc, argv, __FILE__, __PRETTY_FUNCTION__, __LINE__);

  expressionLexer(&Diff);

  Diff.diffTree.root = getG(&Diff);

  // callPrintBinaryTree(&Diff.diffTree, INFIX, stdout);
  
  Differentiator newDiff = {};
  differentiateExpression(&Diff, &newDiff, 'x');

  // DIFF_DUMP_(&Diff.diffTree);
  // DIFF_SAVE_DUMP_IMAGE_(&Diff.diffTree);

  simplifyTree(&newDiff);

  DIFF_DUMP_(&newDiff.diffTree);
  DIFF_SAVE_DUMP_IMAGE_(&newDiff.diffTree);
  // callPrintBinaryTree(&newDiff.diffTree, INFIX, stdout);

  // DIFF_DUMP_(&newDiff.diffTree);
  // DIFF_SAVE_DUMP_IMAGE_(&newDiff.diffTree);
  
  // DIFF_DUMP_(&Diff.diffTree);
  // DIFF_SAVE_DUMP_IMAGE_(&Diff.diffTree);

  return 0;
}
