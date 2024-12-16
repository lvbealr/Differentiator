#include "differentiator.h"
#include "simplifyTree.h"
#include "DSL.h"
#include "customWarning.h"
#include "colorPrint.h"
#include "diffDump.h"

#define NDEBUG

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
                                                          \
    switch (direction) {                                  \
        case LEFT:                                        \
            {                                             \
                parentNode->left = newNode;               \
                break;                                    \
            }                                             \
                                                          \
        case RIGHT:                                       \
            {                                             \
                parentNode->right = newNode;              \
                break;                                    \
            }                                             \
                                                          \
        default:                                          \
            {                                             \
                break;                                    \
            }                                             \
    }                                                     \
                                                          \
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
        newNode = copySubtree(futureNode);
    }

    else {
        newNode = futureNode;
    }

    if ((*formerNode)->parent) {
        if ((*formerNode)->parent->left == (*formerNode)) {
            REPLACE_SUBTREE_(LEFT, newNode);
        }

        else {
            REPLACE_SUBTREE_(RIGHT, newNode);
        }
    }

    else {
        destructSubtree(formerNode);
        diff->diffTree.root = newNode;
    }

    return NO_SIMPLIFY_ERRORS;
}

simplifyError simplifyTree(Differentiator *simpDiff) {
    customWarning(simpDiff != NULL, (simplifyError) TREE_NULL_POINTER);

    $PRINT_FUNC();

    size_t simplificationsNumber = {};

    do {
        simplificationsNumber = 0;
        DIFF_DUMP_(&simpDiff->diffTree);
        simplifySubtree(simpDiff, &(simpDiff->diffTree.root), &simplificationsNumber);
        DIFF_DUMP_(&simpDiff->diffTree);
    } while (simplificationsNumber != 0);

    return NO_SIMPLIFY_ERRORS;
}

simplifyError simplifySubtree(Differentiator *diff, node<diffNode> **rootNode, size_t *simplificationsNumber) {
    customWarning(diff                  != NULL, (simplifyError) DIFF_NULL_PTR);

    if (!(*rootNode)) {
        return (simplifyError) NODE_NULL_PTR;
    }

    customWarning(simplificationsNumber != NULL,                 SIMPLIFICATIONS_NUMBER_NULL_PTR);

    $PRINT_FUNC();
    
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
                        EVAL_FUNCTION;                                                                           \
                    }                                                                                            \
                                                                                                                 \
                    else if (zeroComparison((*rootNode)->left->data.nodeValue.value) == DOUBLE_EQUAL_EPS) {      \
                        LEFT_ZERO_SIMPLIFICATION;                                                                \
                    }                                                                                            \
                                                                                                                 \
                    else if (zeroComparison((*rootNode)->left->data.nodeValue.value - 1) == DOUBLE_EQUAL_EPS) {  \
                        LEFT_ONE_SIMPLIFICATION;                                                                 \
                    }                                                                                            \
                                                                                                                 \
                    else if (zeroComparison((*rootNode)->left->data.nodeValue.value + 1) == DOUBLE_EQUAL_EPS) {  \
                        LEFT_MINUS_ONE_SIMPLIFICATION;                                                           \
                    }                                                                                            \
                }                                                                                                \
                                                                                                                 \
                else if ((*rootNode)->right->data.type == NUMERICAL_NODE) {                                      \
                    if (zeroComparison((*rootNode)->right->data.nodeValue.value) == DOUBLE_EQUAL_EPS) {          \
                        RIGHT_ZERO_SIMPLIFICATION;                                                               \
                    }                                                                                            \
                                                                                                                 \
                    else if (zeroComparison((*rootNode)->right->data.nodeValue.value - 1) == DOUBLE_EQUAL_EPS) { \
                        RIGHT_ONE_SIMPLIFICATION;                                                                \
                    }                                                                                            \
                                                                                                                 \
                    else if (zeroComparison((*rootNode)->right->data.nodeValue.value + 1) == DOUBLE_EQUAL_EPS) { \
                        RIGHT_MINUS_ONE_SIMPLIFICATION;                                                          \
                    }                                                                                            \
                }                                                                                                \
            }                                                                                                    \
                                                                                                                 \
            else if ((*rootNode)->left->data.type == NUMERICAL_NODE) {                                           \
                EVAL_FUNCTION;                                                                                   \
            }                                                                                                    \
        }                                                                                                        \
    }                                                                                                            \

    #include "diffOperations.def"

    DIFF_DUMP_(&diff->diffTree);

    return NO_SIMPLIFY_ERRORS;
}
