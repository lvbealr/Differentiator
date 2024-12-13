#ifndef DSL_H_
#define DSL_H_

#include "differentiator.h"
#include "binaryTree.h"

// FUNCTION PROTOTYPES //
// FUNCTION PROTOTYPES //

// MACROS //    
#define EVAL_SUBTREE_(subtree)                         evalInternal(diff, (rootNode)->subtree)
#define DIFF_(direction)                               NULL // TODO
#define COPY_(subtreeRoot)                             copySubtree   (subtreeRoot)
#define CONST_(number)                                 diffNodeCreate(node<diffNode> {.data       = {.type = NUMERICAL_NODE, \
                                                                                      .nodeValue  = {.value = number}},      \
                                                                                      .left       = NULL,                    \
                                                                                      .right      = NULL,                    \
                                                                                      .parent     = NULL})                   \

#define VARIABLE_(index)                               diffNodeCreate(node<diffNode> {.data       = {.type = VARIABLE_NODE,  \
                                                                                      .nodeValue  = {.varIndex = index}},    \
                                                                                      .left       = NULL,                    \
                                                                                      .right      = NULL,                    \
                                                                                      .parent     = NULL})                   \

#define OPERATION_NODE_(leftNode, rightNode, operator) diffNodeCreate(node<diffNode> {.data       = {.type = OPERATION_NODE, \
                                                                                      .nodeValue  = {.op = operator}},       \
                                                                                      .left       = leftNode,                \
                                                                                      .right      = rightNode,               \
                                                                                      .parent     = NULL})                   \

#define ADD_(leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, ADD )
#define SUB_(leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, SUB )
#define MUL_(leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, MUL )
#define DIV_(leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, DIV )
#define POW_(leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, POW )
#define SIN_(leftNode)            OPERATION_NODE_(leftNode, NULL,      SIN )
#define COS_(leftNode)            OPERATION_NODE_(leftNode, NULL,      COS )
#define SQRT_(leftNode)           OPERATION_NODE_(leftNode, NULL,      SQRT)
#define LN_(leftNode)             OPERATION_NODE_(leftNode, NULL,      LN  )
#define EXP_(leftNode)            OPERATION_NODE_(leftNode, NULL,      EXP )
// MACROS //

#endif // DSL_H_
