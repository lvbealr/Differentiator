#ifndef DIFF_IO_H_
#define DIFF_IO_H_

#include "binaryTree.h"
#include "differentiator.h"

// FUNCTION PROTOTYPES //
template<>
inline binaryTreeError printNode(node<DiffNode> *currentNode, FILE *stream);
// FUNCTION PROTOTYPES //

template<>
inline binaryTreeError printNode(node<DiffNode> *currentNode, FILE *stream) {
  customWarning(currentNode != NULL, NODE_NULL_POINTER);
  customWarning(stream      != NULL, BAD_STREAM_POINTER);

  switch (currentNode->data.type) {
    case NUMERICAL_NODE:
      {
        fprintf(stream, "%lg", currentNode->data.nodeValue.value);
        break;
      }

    case OPERATION_NODE:
      {
        fprintf(stream, "%s", currentNode->data.nodeValue.op.symbol);
        break;
      }

    case VARIABLE_NODE:
      {
        fprintf(stream, "%c", currentNode->data.nodeValue.varIndex);
        break;
      }

    default:
      {
        break;
      }
  }

  return NO_ERRORS;
}


#endif // DIFF_IO_H_