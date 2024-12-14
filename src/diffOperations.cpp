#include "differentiator.h"

#define OPERATOR(NAME, SYMBOL, PRIORITY, ...) {.name = NAME, .symbol = SYMBOL, .priority = PRIORITY},
const operationInfo operations[] = {
    #include "diffOperations.def"
};
#undef OPERATOR

const operationInfo *findOperationBySymbol (const char *symbol) {
    FIND_OPERATION_(!strcmp(operations[opIndex].symbol, symbol));
}

const Variable *findWordInTable(Differentiator *diff, const Variable *word) {
    customWarning(diff != NULL, NULL);
    
    for (size_t index = 0; index < diff->variables->variableCount; index++) {
        if (!strcmp(word->variableName, diff->variables->variableArray[index].variableName)) {
            return &diff->variables->variableArray[index];
        }
    }

    return NULL;
}

// FUNCTION PROTOTYPES //
node<diffNode> *diffNodeCreate(node<diffNode> nodeData);
node<diffNode> *copySubtree   (node<diffNode> *subtreeRoot);
// FUNCTION PROTOTYPES //

node<diffNode> *diffNodeCreate(node<diffNode> nodeData) {
    node<diffNode> *newNode = NULL;
    nodeInitialize(&newNode);

    customWarning(newNode != NULL, NULL);

    newNode->data       = nodeData.data;
    newNode->right      = nodeData.right;
    newNode->left       = nodeData.left;
    newNode->parent     = nodeData.parent;
    newNode->data.depth = 1;

    if (newNode->left) {
        newNode->left->parent = newNode;
        newNode->data.depth  += newNode->left->data.depth;
    }

    if (newNode->right) {
        newNode->right->parent = newNode;
        newNode->data.depth   += newNode->right->data.depth; 
    }

    return newNode;
}

node<diffNode> *copySubtree   (node<diffNode> *subtreeRoot) {
    customWarning(subtreeRoot != NULL, NULL);

    node<diffNode> *newSubtreeRoot = NULL;
    nodeInitialize(&newSubtreeRoot);

    customWarning(newSubtreeRoot != NULL, NULL);

    newSubtreeRoot->data       = subtreeRoot->data;
    newSubtreeRoot->data.depth = subtreeRoot->data.depth;

    #define copySubtreeNode(direction) {                                             \
        if (subtreeRoot->direction) {                                                \
            newSubtreeRoot->direction         = copySubtree(subtreeRoot->direction); \
            newSubtreeRoot->direction->parent = newSubtreeRoot;                      \
        }                                                                            \
    }          

    copySubtreeNode(left);
    copySubtreeNode(right);                                                                         

    #undef copySubtreeNode

    return newSubtreeRoot;
}

