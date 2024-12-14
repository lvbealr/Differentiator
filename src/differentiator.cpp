#include "differentiator.h"
#include "diffIO.h"
#include "binaryTree.h"
#include "DSL.h"
#include "lexer.h"

static const size_t MAX_VARIABLE_COUNT = 3;

diffError diffInitialize(Differentiator *diff, int argc, char *argv[], const char *fileName,
                                                                       const char *funcName,
                                                                       int         line) {
    customWarning(diff != NULL, DIFF_NULL_PTR);

    treeInitialize      (&diff->diffTree);
    treeInfoInitialize  (&diff->diffTree, fileName, funcName, line);
    bufferInitialize    (diff);
    parseConsole        (argc, argv, &diff->diffTree);
    binaryTreeSetInfo   (&diff->diffTree);
    
    diff->buffer->filePath = (char *)"IO/input.txt"; // TODO
    readFromFile(diff);

    diff->tokens = (tokensData *)calloc(1, sizeof(tokensData));
    customWarning(diff->tokens != NULL, BAD_ALLOCATION);
    diff->tokens->tokenArray = (Token *)calloc(diff->buffer->textSize, sizeof(Token)); 
    customWarning(diff->tokens->tokenArray != NULL, BAD_ALLOCATION);

    diff->variables = (variableTable *)calloc(1, sizeof(variableTable));
    customWarning(diff->variables != NULL, BAD_ALLOCATION);
    diff->variables->variableArray = (Variable *)calloc(MAX_VARIABLE_COUNT, sizeof(Variable)); 
    customWarning(diff->variables->variableArray != NULL, BAD_ALLOCATION);

    return NO_DIFF_ERRORS;
}

static double evalInternal(Differentiator *diff, node<diffNode> *rootNode) {
    customWarning(diff     != NULL, DIFF_NULL_PTR);
    customWarning(rootNode != NULL, NODE_NULL_PTR);

    switch (rootNode->data.type) {
        case NUMERICAL_NODE: 
            {
                return rootNode->data.nodeValue.value;
            }
        
        case OPERATION_NODE:
            {
                double evalValue = NAN;

                #define OPERATOR(name, symbol, priority, callFunction, diffFunction)       \
                    if (rootNode->data.nodeValue.op == name) {                             \
                        callFunction                                                       \
                    }

                #include "diffOperations.def"

                #undef OPERATOR

                return evalValue;
            }

        case VARIABLE_NODE:
            {
                      Variable  patternVariable = {.variableName = &rootNode->data.nodeValue.varIndex};
                const Variable *findVariable    = findWordInTable(diff, &patternVariable);
                return findVariable->variableValue;
            }

        default:
            {
                return NAN;
            }
    }

    return NAN;
}

diffError evalTree (Differentiator *diff, double *value) {
    customWarning(diff  != NULL, DIFF_NULL_PTR);
    customWarning(value != NULL, BAD_POINTER);

    *value = evalInternal(diff, diff->diffTree.root);

    return NO_DIFF_ERRORS;
}

