#ifndef RECURSIVE_DESCENT_PARSER_H_
#define RECURSIVE_DESCENT_PARSER_H_

#include "differentiator.h"
#include "colorPrint.h"


// TODO PRINT PART OF STRING (BEFORE AND AFTER)
#define SYNTAX_ERROR(func, line) do {                       \
    customPrint(red, bold, bgDefault, "%s:%d", func, line); \
    exit(-1);                                               \
} while (0) 

// FUNCTION PROTOTYPES //
node<diffNode> *getG   (Differentiator *diff);
node<diffNode> *getE   (Differentiator *diff);
node<diffNode> *getT   (Differentiator *diff);
node<diffNode> *getD   (Differentiator *diff);
node<diffNode> *getP   (Differentiator *diff);
node<diffNode> *getF   (Differentiator *diff);
node<diffNode> *getV   (Differentiator *diff);
node<diffNode> *getN   (Differentiator *diff);
// FUNCTION PROTOTYPES //

#endif // RECURSIVE_DESCENT_PARSER_H_
