#include <cstring>
#include <cctype>

#include "differentiator.h"
#include "recursiveDescentParser.h"
#include "DSL.h"
#include "lexer.h"
#include "diffDump.h"

// #define CHECK_POINTER() {fprintf(stderr, "CHECKER: %s, [%s], %p, %lu\n", __PRETTY_FUNCTION__, diff->tokens->tokenArray[diff->tokens->current].tokenPointer, diff->tokens->tokenArray[diff->tokens->current].tokenPointer, diff->tokens->current);}
// #define PRINT_FUNC()    {fprintf(stderr, "=============================================\n"); fprintf(stderr, "-> %s\n", __PRETTY_FUNCTION__); fprintf(stderr, "=============================================\n\n");}

node<diffNode> *getG(Differentiator *diff) {
    customWarning(diff != NULL, NULL);

    // PRINT_FUNC();

    node<diffNode> *node_ = getE(diff);

    // CHECK_POINTER();

    if (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '\0') {
        SYNTAX_ERROR(__PRETTY_FUNCTION__, __LINE__);
    }
    
    // fprintf(stderr, "[xxx] %s [%c]\n\n", __PRETTY_FUNCTION__, *(diff->tokens->tokenArray[diff->tokens->current].tokenPointer));

    return node_;
}

node<diffNode> *getE(Differentiator *diff) {
    customWarning(diff != NULL, NULL);

    // PRINT_FUNC();

    node<diffNode> *node_       = getT(diff);
    node<diffNode> *resultNode_ = node_;
    node<diffNode> *node__      = NULL;

    // CHECK_POINTER();

    while ((*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '+') ||
        (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '-')) {

        // CHECK_POINTER();

        char operation = *(diff->tokens->tokenArray[diff->tokens->current].tokenPointer);
        // fprintf(stderr, "[xxx] %s [%c]\n", __PRETTY_FUNCTION__, operation);

        diff->tokens->current++;

        node__ = getE(diff);

        // fprintf(stderr, "[xxx] %s OPERATION: %d\n", __PRETTY_FUNCTION__, operation);

        switch (operation) {

            case '+':
                {
                    // fprintf(stderr, "ADD\n");
                    // fprintf(stderr, "[xxx] %s LEFT: [%p %lg], RIGHT: [%p %lg]\n", __PRETTY_FUNCTION__, node_, node_->data.nodeValue.value, node__, node__->data.nodeValue.value);
                    resultNode_ = ADD_(node_, node__);
                    break;
                }
                
            case '-':
                {   
                    // fprintf(stderr, "SUB\n");
                    // fprintf(stderr, "[xxx] %s LEFT: [%p %lu], RIGHT: [%p %lu]\n", __PRETTY_FUNCTION__, node_, node_->data.nodeValue.value, node__, node__->data.nodeValue.value);
                    resultNode_ = SUB_(node_, node__);
                    break;
                }
                
            default:
                {
                    break;
                }
            }
        }

    // fprintf(stderr, "[xxx] %s RESULT: [%p %d], CURRENT: [%lu]\n", __PRETTY_FUNCTION__, resultNode_, resultNode_->data.nodeValue.op, diff->tokens->current);

    return resultNode_;
}

node<diffNode> *getT(Differentiator *diff) {
    customWarning(diff != NULL, NULL);

    // PRINT_FUNC();

    node<diffNode> *node_       = getD(diff);
    node<diffNode> *resultNode_ = node_;
    node<diffNode> *node__      = NULL;

    // CHECK_POINTER();

    while ((*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '*') ||
        (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '/')) {

        // CHECK_POINTER();
        char operation = *(diff->tokens->tokenArray[diff->tokens->current].tokenPointer);

        // fprintf(stderr, "[xxx] %s [%c]\n\n", __PRETTY_FUNCTION__, operation);

        diff->tokens->current++;

        node__ = getT(diff);

        switch (operation) {
            case '*':
                {
                    resultNode_ = MUL_(node_, node__);
                    break;
                }
            
            case '/':
                {
                    resultNode_ = DIV_(node_, node__);
                    break;
                }
            
            default: 
                {
                    break;
                }
        }
    }
    
    // fprintf(stderr, "[xxx] %s RESULT: [%p %d]\n", __PRETTY_FUNCTION__, resultNode_, resultNode_->data.nodeValue.op);

    return resultNode_;
}

node<diffNode> *getD(Differentiator *diff) {
    customWarning(diff != NULL, NULL);

    // PRINT_FUNC();

    node<diffNode> *node_       = getP(diff);
    
    node<diffNode> *resultNode_ = node_;
    node<diffNode> *node__      = NULL;

    // CHECK_POINTER();

    while (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '^') {
        diff->tokens->current++;

        // CHECK_POINTER();

        // fprintf(stderr, "[xxx] %s [%c]\n\n", __PRETTY_FUNCTION__, *diff->tokens->tokenArray[diff->tokens->current - 1].tokenPointer);

        node__ = getD(diff);
        resultNode_ = POW_(node_, node__);
        // fprintf(stderr, "[xxx] %s LEFT NODE: %p RIGHT NODE: %p\n", __PRETTY_FUNCTION__, node_, node__);

        // fprintf(stderr, "[xxx] %s VALUES: %lg %lg\n", __PRETTY_FUNCTION__, node_->data.nodeValue.value, node__->data.nodeValue.value);
    }    

    // fprintf(stderr, "[xxx] %s RESULT_NODE: %p [%lg]\n", __PRETTY_FUNCTION__, resultNode_, resultNode_->data.nodeValue.value);

    return resultNode_;
}

node<diffNode> *getP(Differentiator *diff) {
    customWarning(diff != NULL, NULL);

    // PRINT_FUNC();
    // CHECK_POINTER();

    if (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '(') {

        diff->tokens->current++;

        node<diffNode> *node_ = getE(diff);

        // fprintf(stderr, "%s CURRENT: [%lu]\n", __PRETTY_FUNCTION__, diff->tokens->current);

        // CHECK_POINTER();

        if (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) != ')') {
            SYNTAX_ERROR(__PRETTY_FUNCTION__, __LINE__);
        }

        // diff->tokens->current++;

        return node_;
    }

    else {
        return getF(diff);
    }
}

node<diffNode> *getF(Differentiator *diff) {
    customWarning(diff != NULL, NULL);
    
    // PRINT_FUNC();

    char *funcPattern = (char *)calloc(MAX_WORD_LENGTH, sizeof(char));

    // CHECK_POINTER();

    if (sscanf(diff->tokens->tokenArray[diff->tokens->current].tokenPointer, "%[^(]", funcPattern) != 0) {
        // fprintf(stderr, "[xxx] %s funcPattern: [%s]\n", __PRETTY_FUNCTION__, funcPattern);
    }

    const operationInfo *operation = findOperationBySymbol(funcPattern);
    
    node<diffNode> *node_ = NULL;

    if (!operation) {
        return getV(diff);
    }

    else {
        #define OPERATOR(name, symbol, ...) {                                                        \
            case name:                                                                               \
                {                                                                                    \
                    diff->tokens->current++;                                                         \
                    node_ = getE(diff);                                                              \
                                                                                                     \
                    if (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) != ')') {    \
                        SYNTAX_ERROR(__PRETTY_FUNCTION__, __LINE__);                                 \
                    }                                                                                \
                                                                                                     \
                    diff->tokens->current++;                                                         \
                                                                                                     \
                    return name##_(node_, NULL);                                                     \
                                                                                                     \
                    break;                                                                           \
                }                                                                                    \
        }

        switch (operation->name) {
            #include "diffOperations.def"

            default:
                {
                    break;
                }
        }

        #undef OPERATOR
    }
    
    return NULL;
}

node<diffNode> *getV(Differentiator *diff) {
    customWarning(diff != NULL, NULL);
    
    // PRINT_FUNC();

    char *varName = (char *)calloc(1, sizeof(char));

    // CHECK_POINTER();

    strncpy(varName, diff->tokens->tokenArray[diff->tokens->current].tokenPointer, 1);

    // fprintf(stderr, "[xxx] %s [%s]\n\n", __PRETTY_FUNCTION__, varName);

    const Variable patternWord = {.variableName  = varName,
                                  .variableValue = NAN,
                                  .nextVariable  = NULL};
    
    const Variable *foundWord  = findWordInTable(diff, &patternWord);

    if (foundWord) {
        diff->tokens->current++;
        return VARIABLE_(*foundWord->variableName);
    }

    else {
        return getN(diff);
    }
}

node<diffNode> *getN(Differentiator *diff) {
    customWarning(diff != NULL, NULL);

    // PRINT_FUNC();

    double value = {};

    // CHECK_POINTER();

    if (sscanf(diff->tokens->tokenArray[diff->tokens->current].tokenPointer, "%lg", &value) == 0) {
        SYNTAX_ERROR(__PRETTY_FUNCTION__, __LINE__);
    }

    else {
        // fprintf(stderr, "[xxx] %s [%lg]\n\n", __PRETTY_FUNCTION__, value);
        
        // CHECK_POINTER();

        if (isdigit(*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer))) {
            diff->tokens->current++;

            // CHECK_POINTER();
        }

        else {
            diff->tokens->current += 2;
        }

        return CONST_(value);
    }
}
