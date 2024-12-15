#include <cstring>
#include <cctype>

#include "differentiator.h"
#include "recursiveDescentParser.h"
#include "DSL.h"
#include "lexer.h"
#include "diffDump.h"

#define _NDEBUG

#ifndef _NDEBUG
    #define $CHECK_POINTER()  {customPrint(yellow,    bold, bgDefault, "[CHECKER]: ");                                                \
                               customPrint(lightblue, bold, bgDefault, "(%s), (%p), (%lu), \"%s\"\n",                                 \
                                                                        __PRETTY_FUNCTION__,                                          \
                                                                        diff->tokens->tokenArray[diff->tokens->current].tokenPointer, \
                                                                        diff->tokens->current,                                        \
                                                                        diff->tokens->tokenArray[diff->tokens->current].tokenPointer);}

    #define $PRINT_FUNC()     {customPrint(lightblue, bold, bgDefault, "==========================================\n");               \
                               customPrint(purple,    bold, bgDefault, "->> %s\n", __PRETTY_FUNCTION__);                              \
                               customPrint(lightblue, bold, bgDefault, "==========================================\n");}    

    #define $CHECK_COUNTERS() {customPrint(red, bold, bgDefault, "[COUNTERS]: ");                                                     \
                               customPrint(blue, bold, bgDefault, "%lu ", diff->tokens->current);                                     \
                               customPrint(green, bold, bgDefault, "%lu\n", diff->tokens->count);}

    #define $ALL_CHECK() {$CHECK_POINTER(); \
                          $PRINT_FUNC();    \
                          $CHECK_COUNTERS();}

#else 
    #define $CHECK_POINTER() ;
    #define $PRINT_FUNC()    ;
    #define $CHECK_COUNTER() ;
    #define $ALL_CHECK()     ;

#endif

node<diffNode> *getG(Differentiator *diff) {
    customWarning(diff != NULL, NULL);

    $ALL_CHECK();

    node<diffNode> *node_ = getE(diff);
    
    $ALL_CHECK();

    if (diff->tokens->tokenArray[diff->tokens->current].tokenPointer != NULL) {
        if (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '\0') {
            SYNTAX_ERROR(__PRETTY_FUNCTION__, __LINE__);
        }
    }

    return node_;
}

node<diffNode> *getE(Differentiator *diff) {
    customWarning(diff != NULL, NULL);

    $ALL_CHECK();

    node<diffNode> *node_       = getT(diff);
    node<diffNode> *resultNode_ = node_;
    node<diffNode> *node__      = NULL;

    $ALL_CHECK();

    if (diff->tokens->tokenArray[diff->tokens->current].tokenPointer != NULL) {
        while ((diff->tokens->tokenArray[diff->tokens->current].tokenPointer != NULL) && ((*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '+') ||
            (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '-'))) {

            char operation = *(diff->tokens->tokenArray[diff->tokens->current].tokenPointer);

            diff->tokens->current++;

            $ALL_CHECK();

            node__ = getE(diff);

            $ALL_CHECK();

            switch (operation) {

                case '+':
                    {
                        resultNode_ = ADD_(node_, node__);
                        break;
                    }
                    
                case '-':
                    {   
                        resultNode_ = SUB_(node_, node__);
                        break;
                    }
                    
                default:
                    {
                        break;
                    }
                }
            }
    }

    return resultNode_;
}

node<diffNode> *getT(Differentiator *diff) {
    customWarning(diff != NULL, NULL);

    $ALL_CHECK();

    node<diffNode> *node_       = getD(diff);
    node<diffNode> *resultNode_ = node_;
    node<diffNode> *node__      = NULL;

    $ALL_CHECK();

    if (diff->tokens->count != diff->tokens->current) {
        while ((diff->tokens->tokenArray[diff->tokens->current].tokenPointer != NULL) &&
              ((*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '*') ||
               (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '/'))) {

            char operation = *(diff->tokens->tokenArray[diff->tokens->current].tokenPointer);

            diff->tokens->current++;

            $ALL_CHECK();

            node__ = getT(diff);

            $ALL_CHECK();

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
    }
    
    return resultNode_;
}

node<diffNode> *getD(Differentiator *diff) {
    customWarning(diff != NULL, NULL);

    $ALL_CHECK();
    
    node<diffNode> *node_       = getP(diff);
    node<diffNode> *resultNode_ = node_;
    node<diffNode> *node__      = NULL;

    $ALL_CHECK();

    if (diff->tokens->tokenArray[diff->tokens->current].tokenPointer != NULL) {
        
        $ALL_CHECK();

        while ((diff->tokens->tokenArray[diff->tokens->current].tokenPointer != NULL) &&
               (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '^')) {
            
            diff->tokens->current++;

            $ALL_CHECK();

            node__ = getD(diff);

            $ALL_CHECK();

            resultNode_ = POW_(node_, node__);
        }    
    }

    return resultNode_;
}

node<diffNode> *getP(Differentiator *diff) {
    customWarning(diff != NULL, NULL);

    $ALL_CHECK();

    if ((diff->tokens->tokenArray[diff->tokens->current].tokenPointer) &&
        (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) == '(')) {

        diff->tokens->current++;

        node<diffNode> *node_ = getE(diff);

        $ALL_CHECK();
        $ALL_CHECK();

        if ((diff->tokens->tokenArray[diff->tokens->current].tokenPointer) &&
            (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) != ')')) {
            SYNTAX_ERROR(__PRETTY_FUNCTION__, __LINE__);
        }

        if (!diff->tokens->tokenArray[diff->tokens->current].tokenPointer) {
            SYNTAX_ERROR(__PRETTY_FUNCTION__, __LINE__);

        }

        $ALL_CHECK();

        diff->tokens->current++;
        
        return node_;
    }

    else {
        return getF(diff);
    }
}

node<diffNode> *getF(Differentiator *diff) {
    customWarning(diff != NULL, NULL);
    
    char *funcPattern = (char *)calloc(MAX_WORD_LENGTH, sizeof(char));

    $ALL_CHECK();

    if (sscanf(diff->tokens->tokenArray[diff->tokens->current].tokenPointer, "%[^(]", funcPattern) == 0) {
        SYNTAX_ERROR(__PRETTY_FUNCTION__, __LINE__);
    }
    
    const operationInfo *operation = findOperationBySymbol(funcPattern);
    
    node<diffNode> *node_ = NULL;

    if (!operation) {
        return getV(diff);
    }

    else {
        #define OPERATOR(NAME, ...) {                                                                \
            case NAME:                                                                               \
                {                                                                                    \
                    diff->tokens->current++;                                                         \
                    node_ = getE(diff);                                                              \
                    $ALL_CHECK();                                                                    \
                                                                                                     \
                                                                                                     \
                    if ((diff->tokens->tokenArray[diff->tokens->current].tokenPointer) &&            \
                      (*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer) != ')')) {    \
                        SYNTAX_ERROR(__PRETTY_FUNCTION__, __LINE__);                                 \
                    }                                                                                \
                                                                                                     \
                    $ALL_CHECK();                                                                    \
                                                                                                     \
                    return NAME##_(node_, NULL);                                                     \
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
    
    char *varName = (char *)calloc(1, sizeof(char));

    $ALL_CHECK();
    
    strncpy(varName, diff->tokens->tokenArray[diff->tokens->current].tokenPointer, 1);

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

    double value = {};

    $ALL_CHECK();

    if (sscanf(diff->tokens->tokenArray[diff->tokens->current].tokenPointer, "%lg", &value) == 0) {
        SYNTAX_ERROR(__PRETTY_FUNCTION__, __LINE__);
    }

    else {

        $ALL_CHECK();

        if (isdigit(*(diff->tokens->tokenArray[diff->tokens->current].tokenPointer))) {
            diff->tokens->current++;

            $ALL_CHECK();
        }

        else {
            diff->tokens->current += 2;
        }

        return CONST_(value);
    }
}
