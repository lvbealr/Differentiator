#include "differentiator.h"
#include "diffIO.h"
#include "binaryTree.h"
#include "DSL.h"
#include "lexer.h"
#include "diffDump.h"
#include "colorPrint.h"
#include "simplifyTree.h"

static const size_t MAX_VARIABLE_COUNT = 3;

static void setInputFilePath(Differentiator **diff) {
    customWarning((*diff)                    != NULL, (void) 1);
    customWarning((*diff)->buffer            != NULL, (void) 1);
    customWarning((*diff)->buffer->filePath  != NULL, (void) 1);

    customPrint(green, bold, bgDefault, "Enter Path To Input File: ");
    scanf("%s", (*diff)->buffer->filePath);
}

diffError diffInitialize(Differentiator *diff, int argc, char *argv[], const char *fileName,
                                                                       const char *funcName,
                                                                       int         line) {
    customWarning(diff != NULL, DIFF_NULL_PTR);

    treeInitialize      (&diff->diffTree);
    treeInfoInitialize  (&diff->diffTree, fileName, funcName, line);
    bufferInitialize    (diff);
    parseConsole        (argc, argv, &diff->diffTree);
    binaryTreeSetInfo   (&diff->diffTree);
    
    setInputFilePath(&diff);
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

double evalInternal(Differentiator *diff, node<diffNode> **rootNode) {
    customWarning(diff     != NULL, DIFF_NULL_PTR);
    customWarning((*rootNode) != NULL, NODE_NULL_PTR);

    switch ((*rootNode)->data.type) {
        case NUMERICAL_NODE: 
            {
                customPrint(yellow, bold, bgDefault, "%s %lg\n", __PRETTY_FUNCTION__, (*rootNode)->data.nodeValue.value);
                return (*rootNode)->data.nodeValue.value;
            }
        
        case OPERATION_NODE:
            {   
                customPrint(yellow, bold, bgDefault, "%s %d\n", __PRETTY_FUNCTION__, (*rootNode)->data.nodeValue.op);
                double evalValue = NAN;

                #define OPERATOR(NAME, SYMBOL, PRIORITY, EVAL_FUNCTION, ...)               \
                    if ((*rootNode)->data.nodeValue.op == NAME) {                          \
                        EVAL_FUNCTION                                                      \
                    }

                #include "diffOperations.def"

                #undef OPERATOR

                return evalValue;
            }

        case VARIABLE_NODE:
            {
                customPrint(lightblue, bold, bgDefault, "%c\n", (*rootNode)->data.nodeValue.varIndex);
                      Variable  patternVariable = {.variableName = &(*rootNode)->data.nodeValue.varIndex};
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

    *value = evalInternal(diff, &diff->diffTree.root);

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

    newDiff->diffTree.root = (node<diffNode> *)calloc(1, sizeof(node<diffNode>));
    customPrint(black, bold, bgDefault, "..................%p..................\n", newDiff->diffTree.root);

    newDiff->diffTree.root = differentiateNode(diff, newDiff, &diff->diffTree.root, varIndex);

    customPrint(red, bold, bgDefault, "%p\n", newDiff->diffTree.root);

    return NO_DIFF_ERRORS;
}   

node<diffNode> *differentiateNode(Differentiator *diff, Differentiator *newDiff, node<diffNode> **rootNode, char varIndex) {
    customWarning(diff                   != NULL, NULL);
    customWarning(newDiff                != NULL, NULL);
    customWarning((*rootNode)            != NULL, NULL);

    customPrint(green, bold, bgDefault, "Я ЗДЕСЬ\n");

    if ((*rootNode)->data.type == NUMERICAL_NODE || ((*rootNode)->data.type == VARIABLE_NODE && (*rootNode)->data.nodeValue.varIndex != varIndex)) {
        return CONST_(0);
    }

    if ((*rootNode)->data.type == VARIABLE_NODE && (*rootNode)->data.nodeValue.varIndex == varIndex) {
        customPrint(yellow, bold, bgDefault, "СОЗДАЛ ЕДИНИЧКУ\n");
        return CONST_(1);
    }

    customPrint(red, bold, bgDefault, "НИЧЕГО НЕ СОЗДАЛ\n");

    node<diffNode> *currentNode = NULL;

    #define OPERATOR(NAME, SYMBOL, PRIORITY, EVAL_FUNCTION, DIFF_FUNCTION, ...)               \
        if ((*rootNode)->data.nodeValue.op == NAME) {                                         \
            customPrint(red, bold, bgDefault, "================ START ================\n");   \
            customPrint(purple, bold, bgDefault, "NODE: %s\n", #NAME);                        \
                                                                                              \
            DIFF_FUNCTION                                                                     \
            customPrint(purple, bold, bgDefault, "ПРОШЕЛ, %p", currentNode);                  \
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
