#ifndef DIFF_IO_H_
#define DIFF_IO_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "binaryTree.h"
#include "differentiator.h"
#include "customWarning.h"

const size_t MAX_FILEPATH_SIZE = 100;
const size_t MAX_BUFFER_SIZE   = 300;

// FUNCTION PROTOTYPES //
diffError readFromFile(Differentiator *diff);

template<>
inline binaryTreeError printNode(node<diffNode> *currentNode, FILE *stream);
// FUNCTION PROTOTYPES //

template<>
inline binaryTreeError printNode(node<diffNode> *currentNode, FILE *stream) {
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
        #define OPERATOR(NAME, SYMBOL, ...) {           \
          if (currentNode->data.nodeValue.op == NAME) { \
            fprintf(stream, "%s", SYMBOL);              \
          }                                             \
        }

        #include "diffOperations.def"

        #undef OPERATOR
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
