#ifndef DSL_H_
#define DSL_H_

#include "differentiator.h"
#include "binaryTree.h"

// FUNCTION PROTOTYPES //
// FUNCTION PROTOTYPES //

// MACROS //    
#define EVAL_SUBTREE_(subtree)                         evalInternal(diff, subtree)
#define DIFF_(direction)                               differentiateNode(diff, newDiff, direction, 'x')
#define COPY_(subtreeRoot)                             copySubtree(subtreeRoot)
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

#define ADD_(leftNode,  rightNode) OPERATION_NODE_(leftNode, rightNode, ADD )
#define SUB_(leftNode,  rightNode) OPERATION_NODE_(leftNode, rightNode, SUB )
#define MUL_(leftNode,  rightNode) OPERATION_NODE_(leftNode, rightNode, MUL )
#define DIV_(leftNode,  rightNode) OPERATION_NODE_(leftNode, rightNode, DIV )
#define POW_(leftNode,  rightNode) OPERATION_NODE_(leftNode, rightNode, POW )
#define SIN_(leftNode,  rightNode) OPERATION_NODE_(leftNode, rightNode, SIN )
#define COS_(leftNode,  rightNode) OPERATION_NODE_(leftNode, rightNode, COS )
#define SQRT_(leftNode, rightNode) OPERATION_NODE_(leftNode, rightNode, SQRT)
#define LN_(leftNode,   rightNode) OPERATION_NODE_(leftNode, rightNode, LN  )
#define EXP_(leftNode,  rightNode) OPERATION_NODE_(leftNode, rightNode, EXP )
#define OPEN_BRACKET_(...)  NULL
#define CLOSE_BRACKET_(...) NULL
#define EQUALS_(...)        NULL
// MACROS //

#endif // DSL_H_
