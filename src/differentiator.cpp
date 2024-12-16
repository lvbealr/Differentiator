#include "differentiator.h"
#include "diffIO.h"
#include "binaryTree.h"
#include "DSL.h"
#include "lexer.h"
#include "diffDump.h"
#include "colorPrint.h"
#include "simplifyTree.h"
#include "latex.h"

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

    diff->latexPath = (char *)calloc(MAX_DUMP_FILE_NAME, sizeof(char));
    customWarning(diff->latexPath != NULL, BAD_ALLOCATION);

    return NO_DIFF_ERRORS;
}

diffError diffDestruct(Differentiator *diff) {
    customWarning(diff != NULL, DIFF_NULL_PTR);

    diffTablesDestruct    (diff);
    bufferDestruct        (diff);
    
    treeDestruct          (&diff->diffTree);
    
    FREE_(diff->latexPath);

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

diffError diffTablesDestruct(Differentiator *diff) {
    customWarning(diff            != NULL, (diffError) DIFF_NULL_PTR);
    customWarning(diff->tokens    != NULL, (diffError) TOKENS_TABLE_ERROR);
    customWarning(diff->variables != NULL, (diffError) VARIABLE_TABLE_ERROR);

    FREE_(diff->variables->variableArray);
    diff->variables->variableCount = 0;
    FREE_(diff->variables);

    FREE_(diff->tokens->tokenArray);
    diff->tokens->count   = 0;
    diff->tokens->current = 0;
    FREE_(diff->tokens);

    return NO_DIFF_ERRORS;
}

double evalInternal(Differentiator *diff, node<diffNode> **rootNode) {
    customWarning(diff     != NULL, DIFF_NULL_PTR);
    customWarning((*rootNode) != NULL, NODE_NULL_PTR);

    switch ((*rootNode)->data.type) {
        case NUMERICAL_NODE: 
            {
                return (*rootNode)->data.nodeValue.value;
            }
        
        case OPERATION_NODE:
            {   
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

diffError differentiateExpression(Differentiator *diff, Differentiator *newDiff, char varIndex, FILE *latexStream) {
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
    newDiff->diffTree.root = differentiateNode(diff, newDiff, &diff->diffTree.root, varIndex, latexStream);

    return NO_DIFF_ERRORS;
}   

node<diffNode> *differentiateNode(Differentiator *diff, Differentiator *newDiff, node<diffNode> **rootNode, char varIndex, FILE *latexStream) {
    customWarning(diff                   != NULL, NULL);
    customWarning(newDiff                != NULL, NULL);
    customWarning((*rootNode)            != NULL, NULL);

    node<diffNode> *currentNode = NULL;

    if ((*rootNode)->data.type == NUMERICAL_NODE || ((*rootNode)->data.type == VARIABLE_NODE && (*rootNode)->data.nodeValue.varIndex != varIndex)) {
        currentNode = CONST_(0);
        writePhrase    (derivatePhrase, latexStream, PHRASES_COUNT);
        writeDerivating((*rootNode), "$$(", ")'");
        writeDerivating(currentNode, " = ", "$$\n");
    }

    if ((*rootNode)->data.type == VARIABLE_NODE && (*rootNode)->data.nodeValue.varIndex == varIndex) {
        currentNode = CONST_(1);
        writePhrase    (derivatePhrase, latexStream, PHRASES_COUNT);
        writeDerivating((*rootNode), "$$(", ")'");
        writeDerivating(currentNode, " = ", "$$\n");
    }

    #define OPERATOR(NAME, SYMBOL, PRIORITY, EVAL_FUNCTION, DIFF_FUNCTION, ...)               \
        if ((*rootNode)->data.nodeValue.op == NAME) {                                         \
            DIFF_FUNCTION                                                                     \
                                                                                              \
            if (currentNode->left) {                                                          \
                currentNode->left->parent  = currentNode;                                     \
            }                                                                                 \
            if (currentNode->right) {                                                         \
                currentNode->right->parent = currentNode;                                     \
            }                                                                                 \
                                                                                              \
            writePhrase    (derivatePhrase, latexStream, PHRASES_COUNT);                      \
            writeDerivating((*rootNode), "$$(", ")'");                                        \
            writeDerivating(currentNode, " = ", "$$\n");                                      \
                                                                                              \
            DIFF_DUMP_(&newDiff->diffTree);                                                   \
        }                                                                    

    #include "diffOperations.def"

    #undef OPERATOR

    return currentNode;
}
