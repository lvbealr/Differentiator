#ifndef DIFFERENTIATOR_H_
#define DIFFERENTIATOR_H_

#include <cstdlib>
#include <cmath>

#include "binaryTree.h"

#define FIND_OPERATION_(...) do {                                      \
    for (size_t opIndex = 0; opIndex < OPERATIONS_NUMBER; opIndex++) { \
        if (__VA_ARGS__) {                                             \
            return operations + opIndex;                               \
        }                                                              \
    }                                                                  \
                                                                       \
    return NULL;                                                       \
} while (0)

const size_t MAX_PRIORITY = 4;

enum diffError {
  NO_DIFF_ERRORS    =      0,
  DIFF_NULL_PTR     = 1 << 0,
  TREE_ERROR        = 1 << 1,
  NODE_NULL_PTR     = 1 << 2,
  WRONG_OPERATION   = 1 << 3,
  NO_VALUE          = 1 << 4,
  FILE_ERROR        = 1 << 5,
  DUMP_FILE_ERROR   = 1 << 6,
  BUFFER_ERROR      = 1 << 7,
  BAD_ALLOCATION    = 1 << 8,
  BAD_POINTER       = 1 << 9
};

enum nodeType {
  NUMERICAL_NODE = -2,
  OPERATION_NODE = -1,
  VARIABLE_NODE  =  0
};

#define OPERATOR(NAME, ...) NAME,
enum Operation {
  EMPTY_OPERATOR,
  #include "diffOperations.def"
};
#undef OPERATOR

#define OPERATOR(...) 1 +
const size_t OPERATIONS_NUMBER =
                               #include "diffOperations.def"
                               0;
#undef OPERATOR

struct operationInfo {
  Operation   name     = {};
  const char *symbol   = {};
  size_t      priority = 0;
};

union nodeData {
  double           value;
  char             varIndex;
  Operation        op;
};

struct diffNode {
  nodeType  type       = NUMERICAL_NODE;
  nodeData  nodeValue  = {.value = NAN};
};

struct Variable {
  char     *variableName  = {};
  double    variableValue = NAN;
  Variable *nextVariable  = NULL;
};

struct variableTable {
  size_t    variableCount = 0;
  Variable *variableArray = NULL; 
};

struct Buffer {
  char  *filePath = {};
  char  *text     = {};
  size_t textSize = {};
};

// TODO move to recursiveDescentParser.h

typedef nodeType tokenType;

struct Token {
  char      *tokenPointer = NULL;
  tokenType  type         = {};
  Token     *nextToken    = NULL;
};

struct tokensData {
  size_t current    = 0;
  size_t count      = 0;
  Token *tokenArray = NULL;
};

struct Differentiator {
  binaryTree<diffNode> diffTree    =             {};
  variableTable       *variables   =             {};
  diffError            errorStatus = NO_DIFF_ERRORS;
  Buffer              *buffer      =             {};
  tokensData          *tokens      =             {};
  char                *latexPath   =             {};
};

// FUNCTION PROTOTYPES //
diffError diffInitialize      (Differentiator *diff, int argc, char *argv[], const char *filename,
                                                                             const char *funcName,
                                                                             int         line);
diffError diffDestruct        (Differentiator *diff);

diffError diffTablesInitialize(Differentiator *diff);
diffError diffTablesDestruct  (Differentiator *diff);
diffError bufferInitialize    (Differentiator *diff);

diffError evalTree            (Differentiator *diff, double *value);
double    evalInternal        (Differentiator *diff, node<diffNode> **rootNode);

const operationInfo *findOperationBySymbol(const char *symbol);
const Variable      *findWordInTable      (Differentiator *diff, const Variable *word);

diffError       differentiateExpression	  (Differentiator *diff, Differentiator *newDiff, char varIndex, FILE *latexStream);
node<diffNode> *differentiateNode	   	    (Differentiator *diff, Differentiator *newDiff, node<diffNode> **rootNode, char varIndex, FILE *latexStream);

// FUNCTION FOR NODES //
node<diffNode> *diffNodeCreate(node<diffNode> nodeData);
node<diffNode> *copySubtree   (node<diffNode> *subtreeRoot);
// FUNCTION FOR NODES //

// FUNCTION PROROTYPES //

#endif // DIFFERENTIATOR_H_
