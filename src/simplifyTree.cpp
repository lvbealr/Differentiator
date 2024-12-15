#include "differentiator.h"
#include "simplifyTree.h"
#include "DSL.h"
#include "customWarning.h"
#include "colorPrint.h"
#include "diffDump.h"

#ifndef NDEBUG
    #define $PRINT_FUNC()     {customPrint(lightblue, bold, bgDefault, "==========================================\n");               \
                               customPrint(purple,    bold, bgDefault, "->> %s\n", __PRETTY_FUNCTION__);                              \
                               customPrint(lightblue, bold, bgDefault, "==========================================\n");}
#else
    #define $PRINT_FUNC()
#endif

#define REPLACE_SUBTREE_(direction, newNode)              \
    node<diffNode> *parentNode = (*formerNode)->parent;   \
    DIFF_DUMP_(&(diff->diffTree));                        \
    destructSubtree(formerNode);                          \
    nodeLink(&(diff->diffTree), parentNode, direction  ); \
    switch (direction) {                                  \
        case LEFT:                                        \
            {                                             \
                parentNode->left = newNode;               \
                break;                                    \
            }                                             \
        case RIGHT:                                       \
            {                                             \
                parentNode->right = newNode;              \
                break;                                    \
            }                                             \
        default: break;                                   \
    }                                                     \
    newNode->parent = parentNode;                         

zeroComparisonCode zeroComparison(double x) {
    if (x < -EPS) {
        return DOUBLE_LESS_EPS;
    }
    else if (fabs(x) <= EPS) {
        return DOUBLE_EQUAL_EPS;
    }
    else {
        return DOUBLE_GREATER_EPS;
    }
}

binaryTreeError destructSubtree(node<diffNode> **formerNode) {
    customWarning((*formerNode) != NULL, NODE_NULL_POINTER);

    $PRINT_FUNC();

    if ((*formerNode)->left) {
        destructSubtree(&(*formerNode)->left);
    }

    if ((*formerNode)->right) {
        destructSubtree(&(*formerNode)->right);
    }

    FREE_(*formerNode);

    return NO_ERRORS;
}

simplifyError replaceNode(Differentiator *diff, node<diffNode> **formerNode, node<diffNode> *futureNode) {
    customWarning( diff        != NULL, (simplifyError) TREE_NULL_POINTER);
    customWarning(*formerNode  != NULL, (simplifyError) NODE_NULL_POINTER);
    customWarning( futureNode  != NULL, (simplifyError) NODE_NULL_POINTER);

    $PRINT_FUNC();

    node<diffNode> *newNode = NULL;

    DIFF_DUMP_(&diff->diffTree);

    if (futureNode->parent) {
        customPrint(red, bold, bgDefault, "COPY SUBTREE\n");
        newNode = copySubtree(futureNode);
    }

    else {
        customPrint(red, bold, bgDefault, "NO COPY SUBTREE\n");
        newNode = futureNode;
    }

    customPrint(red, bold, bgDefault, "SET PARENT %s\n", __PRETTY_FUNCTION__);

    if ((*formerNode)->parent) {
        if ((*formerNode)->parent->left == (*formerNode)) {
            customPrint(yellow, bold, bgDefault, "CHECK 1\n");
            REPLACE_SUBTREE_(LEFT, newNode);
            customPrint(yellow, bold, bgDefault, "CHECK 2\n");
        }

        else {
            customPrint(yellow, bold, bgDefault, "CHECK 3\n");
            REPLACE_SUBTREE_(RIGHT, newNode);
            customPrint(yellow, bold, bgDefault, "CHECK 4\n");
        }
    }

    else {
        customPrint(lightblue, bold, bgDefault, "DESTRUCT SUBTREE\n");
        destructSubtree(formerNode);
        diff->diffTree.root = newNode;
    }

    customPrint(red, bold, bgDefault, "REPLACED\n");

    return NO_SIMPLIFY_ERRORS;
}

simplifyError simplifyTree(Differentiator *simpDiff) {
    customWarning(simpDiff != NULL, (simplifyError) TREE_NULL_POINTER);

    $PRINT_FUNC();

    size_t simplificationsNumber = {};

    do {
        simplificationsNumber = 0;
        DIFF_DUMP_(&simpDiff->diffTree);
        customPrint(red, bold, bgDefault, "ЗАХОЖУ В SIMP SUBTREE: %p, %d\n", simpDiff->diffTree.root, simpDiff->diffTree.root->data.type);
        simplifySubtree(simpDiff, &(simpDiff->diffTree.root), &simplificationsNumber);
        DIFF_DUMP_(&simpDiff->diffTree);
        customPrint(yellow, bold, bgDefault, "SIMPLIFICATIONS NUMBER: %lu\n", simplificationsNumber);
    } while (simplificationsNumber != 0);

    return NO_SIMPLIFY_ERRORS;
}

simplifyError simplifySubtree(Differentiator *diff, node<diffNode> **rootNode, size_t *simplificationsNumber) {
    customWarning(diff                  != NULL, (simplifyError) DIFF_NULL_PTR);
    customWarning((*rootNode)           != NULL, (simplifyError) NODE_NULL_PTR);
    customWarning(simplificationsNumber != NULL,                 SIMPLIFICATIONS_NUMBER_NULL_PTR);

    $PRINT_FUNC();

    customPrint(lightblue, bold, bgDefault, "%d: ROOT BEFORE [%p]\n", __LINE__, (*rootNode));

    switch ((*rootNode)->data.type) {
        case NUMERICAL_NODE:
            {
                customPrint(lightblue, bold, bgDefault, "NUMBER: %lg\n", (*rootNode)->data.nodeValue.value);
                break;
            }
        case OPERATION_NODE:
            {
                customPrint(lightblue, bold, bgDefault, "OPERATION: %d\n", (*rootNode)->data.nodeValue.op);
                break;
            }

        case VARIABLE_NODE:
            {
                customPrint(lightblue, bold, bgDefault, "VARIABLE %c\n", (*rootNode)->data.nodeValue.varIndex);
                break;
            }

        default:
            {
                break;
            }
    }


    if (!(*rootNode)) {
        DIFF_DUMP_(&diff->diffTree);

        return NO_SIMPLIFY_ERRORS;
    }

    DIFF_DUMP_(&diff->diffTree);
    simplifySubtree(diff, &(*rootNode)->left,  simplificationsNumber);
    simplifySubtree(diff, &(*rootNode)->right, simplificationsNumber);
    DIFF_DUMP_(&diff->diffTree);

    double evalValue = {};

    #define OPERATOR(NAME, SYMBOL, PRIORITY, EVAL_FUNCTION, DIFF_FUNCTION, LEFT_ZERO_SIMPLIFICATION,             \
                                                                           RIGHT_ZERO_SIMPLIFICATION,            \
                                                                           LEFT_ONE_SIMPLIFICATION,              \
                                                                           RIGHT_ONE_SIMPLIFICATION,             \
                                                                           LEFT_MINUS_ONE_SIMPLIFICATION,        \
                                                                           RIGHT_MINUS_ONE_SIMPLIFICATION)       \
                                                                                                                 \
    if ((*rootNode)->data.type == OPERATION_NODE && (*rootNode)->data.nodeValue.op == NAME) {                    \
        if ((*rootNode)->left) {                                                                                 \
            if ((*rootNode)->right) {                                                                            \
                if ((*rootNode)->left->data.type == NUMERICAL_NODE) {                                            \
                    if ((*rootNode)->right->data.type == NUMERICAL_NODE) {                                       \
                        customPrint(green, bold, bgDefault, "EVAL_FUNCTION 1\n");                                \
                        EVAL_FUNCTION;                                                                           \
                        customPrint(green, bold, bgDefault, "OK EVAL_FUNCTION 1\n");                             \
                    }                                                                                            \
                                                                                                                 \
                    else if (zeroComparison((*rootNode)->left->data.nodeValue.value) == DOUBLE_EQUAL_EPS) {      \
                                            customPrint(green, bold, bgDefault, "LEFT_ZERO\n");                  \
                        LEFT_ZERO_SIMPLIFICATION;                                                                \
                        customPrint(green, bold, bgDefault, "OK LEFT_ZERO\n");                                   \
                    }                                                                                            \
                                                                                                                 \
                    else if (zeroComparison((*rootNode)->left->data.nodeValue.value - 1) == DOUBLE_EQUAL_EPS) {  \
                                            customPrint(green, bold, bgDefault, "LEFT_ONE\n");                   \
                        LEFT_ONE_SIMPLIFICATION;                                                                 \
                        customPrint(green, bold, bgDefault, "OK LEFT_ONE\n");                                    \
                    }                                                                                            \
                                                                                                                 \
                    else if (zeroComparison((*rootNode)->left->data.nodeValue.value + 1) == DOUBLE_EQUAL_EPS) {  \
                                            customPrint(green, bold, bgDefault, "LEFT_MINUS_ONE\n");             \
                        LEFT_MINUS_ONE_SIMPLIFICATION;                                                           \
                        customPrint(green, bold, bgDefault, "OK LEFT_MINUS_ONE\n");                              \
                    }                                                                                            \
                }                                                                                                \
                                                                                                                 \
                else if ((*rootNode)->right->data.type == NUMERICAL_NODE) {                                      \
                    if (zeroComparison((*rootNode)->right->data.nodeValue.value) == DOUBLE_EQUAL_EPS) {          \
                                            customPrint(green, bold, bgDefault, "RIGHT_ZERO\n");                 \
                        RIGHT_ZERO_SIMPLIFICATION;                                                               \
                                            customPrint(green, bold, bgDefault, "OK RIGHT_ZERO\n");              \
                    }                                                                                            \
                                                                                                                 \
                    else if (zeroComparison((*rootNode)->right->data.nodeValue.value - 1) == DOUBLE_EQUAL_EPS) { \
                                            customPrint(green, bold, bgDefault, "RIGHT_ONE\n");                  \
                        RIGHT_ONE_SIMPLIFICATION;                                                                \
                        customPrint(green, bold, bgDefault, "OK RIGHT_ONE\n");                                   \
                    }                                                                                            \
                                                                                                                 \
                    else if (zeroComparison((*rootNode)->right->data.nodeValue.value + 1) == DOUBLE_EQUAL_EPS) { \
                                            customPrint(green, bold, bgDefault, "RIGHT_MINUS_ONE\n");            \
                        RIGHT_MINUS_ONE_SIMPLIFICATION;                                                          \
                        customPrint(green, bold, bgDefault, "OK RIGHT_MINUS_ONE\n");                             \
                    }                                                                                            \
                }                                                                                                \
            }                                                                                                    \
                                                                                                                 \
            else if ((*rootNode)->left->data.type == NUMERICAL_NODE) {                                           \
                                    customPrint(green, bold, bgDefault, "EVAL_FUNCTION 2");                      \
                EVAL_FUNCTION;                                                                                   \
                customPrint(green, bold, bgDefault, "OK EVAL_FUNCTION 2");                                       \
            }                                                                                                    \
        }                                                                                                        \
    }                                                                                                            \

    #include "diffOperations.def"

    customPrint(green, bold, bgDefault, "%d: ROOT AFTER [%p]\n", __LINE__, (*rootNode));

    switch ((*rootNode)->data.type) {
        case NUMERICAL_NODE:
            {
                customPrint(lightblue, bold, bgDefault, "NUMBER: %lg\n", (*rootNode)->data.nodeValue.value);
                break;
            }
        case OPERATION_NODE:
            {
                customPrint(lightblue, bold, bgDefault, "OPERATION: %d\n", (*rootNode)->data.nodeValue.op);
                break;
            }

        case VARIABLE_NODE:
            {
                customPrint(lightblue, bold, bgDefault, "VARIABLE %c\n", (*rootNode)->data.nodeValue.varIndex);
                break;
            }

        default:
            {
                break;
            }
    }

    #undef OPERATOR

    DIFF_DUMP_(&diff->diffTree);

    customPrint(purple, bold, bgDefault, "EVAL VALUE: %lg\n", evalValue);
    
    return NO_SIMPLIFY_ERRORS;
}
