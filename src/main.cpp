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

  diffInitialize (&Diff, argc, argv, __FILE__, __PRETTY_FUNCTION__, __LINE__);
  expressionLexer(&Diff);

  Diff.diffTree.root = getG(&Diff);

  Differentiator newDiff = {};
  differentiateExpression(&Diff, &newDiff, 'x');

  callPrintBinaryTree(&newDiff.diffTree, INFIX, stdout);

  simplifyTree(&newDiff);

  DIFF_DUMP_           (&newDiff.diffTree);
  DIFF_SAVE_DUMP_IMAGE_(&newDiff.diffTree);

  diffDestruct(&Diff);
  // diffDestruct(&newDiff); // TODO

  return 0;
}
