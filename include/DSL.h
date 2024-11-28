#ifndef DSL_H_
#define DSL_H_

#include "differentiator.h"
#include "binaryTree.h"

// FUNCTION PROTOTYPES //
node<diffNode> *createNode  (node<diffNode>  node);
node<diffNode> *copySubtree (node<diffNode> *subtreeRoot);
// FUNCTION PROTOTYPES //

// MACROS //
#define EVAL_SUBTREE_ (subtree) evalInside    (diff, root->subtree);

#define DIFF_           (subtree) diffInside  (diff);
#define COPY_           (subtree) copySubtree (root->subtree);

#define CONST_          (number)  createNode  (node<diffNode> {.left = NULL, .right = NULL, .parent = NULL,                  \
                                                  .nodeData = {.type = NUMERICAL_NODE, .nodeValue = {.value = number}}})

#define VARIABLE_       (number)  createNode  (node<diffNode> {.left = NULL, .right = NULL, .parent = NULL,                  \
                                                  .nodeData = {.type = VARIABLE_NODE, .nodeValue = {.varIndex = index}}})

#define OPERATION_NODE_ (leftNode, rightNode, operator)                                                                      \
                                  createNode  (node<diffNode> {.left = NULL, .right = NULL, .parent = NULL,                  \
                                                  .nodeData = {.type = OPERATION_NODE, .nodeValue = {.operation = operator}}})

#define POW_            (leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, POW );
#define SIN_            (leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, SIN );
#define COS_            (leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, COS );
#define SQRT_           (leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, SQRT);
#define LN_             (leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, LN  );
#define EXP_            (leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, EXP );
#define MUL_            (leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, MUL );
#define DIV_            (leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, DIV );
#define ADD_            (leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, ADD );
#define SUB_            (leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, SUB );
// MACROS //

#endif // DSL_H_