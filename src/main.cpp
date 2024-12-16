#include <cstdio>

#include "differentiator.h"
#include "diffDump.h"
#include "diffIO.h"
#include "DSL.h"
#include "lexer.h"
#include "recursiveDescentParser.h"
#include "simplifyTree.h"
#include "latex.h"

int main(int argc, char *argv[]) {
  Differentiator Diff = {};

  diffInitialize (&Diff, argc, argv, __FILE__, __PRETTY_FUNCTION__, __LINE__);
  
  expressionLexer(&Diff);

  Diff.diffTree.root = getG(&Diff);

  FILE *fileStream = writeToLatex(&Diff, Diff.diffTree.root);

  simplifyTree(&Diff, fileStream, MAIN_FUNCTION);

  Differentiator newDiff = {};
  differentiateExpression(&Diff, &newDiff, 'x', fileStream);

  DIFF_SAVE_DUMP_IMAGE_(&Diff.diffTree);

  simplifyTree(&newDiff, fileStream, DERIVATIVE_FUNCTION);

  writeEnding(&Diff, fileStream);

  DIFF_DUMP_           (&newDiff.diffTree);
  DIFF_SAVE_DUMP_IMAGE_(&newDiff.diffTree);

  // diffDestruct(&Diff);

  return 0;
}
