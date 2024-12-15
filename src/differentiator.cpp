#include "differentiator.h"
#include "diffIO.h"
#include "binaryTree.h"
#include "DSL.h"
#include "lexer.h"
#include "diffDump.h"
#include "colorPrint.h"

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

    diffTablesInitialize(diff);

    return NO_DIFF_ERRORS;
}

diffError diffTablesInitialize(Differentiator *diff) {
    customWarning(diff != NULL, DIFF_NULL_PTR);

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

                #define OPERATOR(NAME, SYMBOL, PRIORITY, EVAL_FUNCTION, ...)               \
                    if (rootNode->data.nodeValue.op == NAME) {                             \
                        EVAL_FUNCTION                                                      \
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

diffError evalTree(Differentiator *diff, double *value) {
    customWarning(diff  != NULL, DIFF_NULL_PTR);
    customWarning(value != NULL, BAD_POINTER);

    *value = evalInternal(diff, diff->diffTree.root);

    return NO_DIFF_ERRORS;
}

diffError differentiateExpression(Differentiator *diff, Differentiator *newDiff, char varIndex) {
    customWarning(diff    != NULL, DIFF_NULL_PTR);
    customWarning(newDiff != NULL, DIFF_NULL_PTR);
    
    treeInitialize    (&newDiff->diffTree);
    treeInfoInitialize(&newDiff->diffTree, __FILE__, __PRETTY_FUNCTION__, __LINE__);
    bufferInitialize  (newDiff);
    newDiff->diffTree.infoData = (binaryTreeInfo *)calloc(1, sizeof(binaryTreeInfo));
    customWarning(newDiff->diffTree.infoData != NULL, TREE_ERROR);

    newDiff->diffTree.infoData = diff->diffTree.infoData;

    binaryTreeSetInfo(&newDiff->diffTree);

    diffTablesInitialize(newDiff);
    newDiff->variables = diff->variables;

    FREE_(newDiff->diffTree.root);

    newDiff->diffTree.root = differentiateNode(diff ,newDiff, diff->diffTree.root, varIndex);

    return NO_DIFF_ERRORS;
}   

node<diffNode> *differentiateNode(Differentiator *diff, Differentiator *newDiff, node<diffNode> *expRoot, char varIndex) {
    customWarning(diff                   != NULL, NULL);
    customWarning(newDiff                != NULL, NULL);

    if (expRoot->data.type == NUMERICAL_NODE || (expRoot->data.type == VARIABLE_NODE && expRoot->data.nodeValue.varIndex != varIndex)) {
        return CONST_(0);
    }

    if (expRoot->data.type == VARIABLE_NODE && expRoot->data.nodeValue.varIndex == varIndex) {
        return CONST_(1);
    }

    node<diffNode> *currentNode = NULL;

    #define OPERATOR(NAME, SYMBOL, PRIORITY, EVAL_FUNCTION, DIFF_FUNCTION)                    \
        if (expRoot->data.nodeValue.op == NAME) {                                             \
            customPrint(red, bold, bgDefault, "================ START ================\n");   \
            customPrint(purple, bold, bgDefault, "NODE: %s\n", #NAME);                        \
                                                                                              \
            DIFF_FUNCTION                                                                     \
            fprintf(stderr, "%p", currentNode);                                               \
                                                                                              \
            if (currentNode->left) {                                                          \
                currentNode->left->parent  = currentNode;                                     \
            }                                                                                 \
            if (currentNode->right) {                                                         \
                currentNode->right->parent = currentNode;                                     \
            }                                                                                 \
                                                                                              \
            DIFF_DUMP_(&newDiff->diffTree);                                                   \
            customPrint(green, bold, bgDefault, "================= END =================\n"); \
            return currentNode;                                                               \
        }                                                                    

    #include "diffOperations.def"

    #undef OPERATOR

    return NULL;
}
