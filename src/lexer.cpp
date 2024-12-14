#include <cstdio>
#include <cctype>
#include "customWarning.h"
#include "differentiator.h"
#include "recursiveDescentParser.h"
#include "DSL.h"
#include "lexer.h"

parseError expressionLexer(Differentiator *diff) {
    if (diff == NULL) {
        return DIFF_PTR_ERROR;
    }
    
    char *symbol = diff->buffer->text;
    char *word   = (char *)calloc(MAX_WORD_LENGTH + 1, sizeof(char));
    customWarning(word != NULL, (parseError) BAD_ALLOCATION); // TODO it works?
    
    while (symbol < diff->buffer->text + diff->buffer->textSize) {
    
        if (isspace(*symbol)) {
            symbol++;
            continue;
        }
        
        if (isdigit(*symbol)) {
            double number       = NAN;
            size_t numberLength = 0;

            if (sscanf(symbol, "%lg%n", &number, &numberLength) != 0) {
                // fprintf(stderr, "[%lg]\n", number); // TODO
                node<diffNode> *nodePointer = CONST_(number);
                
                if (diff->tokens) {
                    diff->tokens->tokenArray[diff->tokens->count++] = {.tokenPointer = symbol,
                                                                       .type         = NUMERICAL_NODE,
                                                                       .nextToken    = NULL};
                    // fprintf(stderr, "tokenArray[%lu] = %s\n", diff->tokens->count - 1, diff->tokens->tokenArray[diff->tokens->count - 1].tokenPointer); // TODO
                    if (diff->tokens->count - 1 > 0) {
                        diff->tokens->tokenArray[diff->tokens->count - 1].nextToken = 
                       &diff->tokens->tokenArray[diff->tokens->count];
                    }
                } // TODO FUNCTION CREATE TOKEN?
                
                symbol += numberLength;
                continue;
            }
        }

        size_t letterNumber = 0;
        bool   isOperation  = false;
        
        while (symbol < diff->buffer->text + diff->buffer->textSize && (isalpha(*symbol) || ispunct(*symbol))) {
            word[letterNumber]     = *symbol;
            word[letterNumber + 1] = '\0';

            symbol++;

            const operationInfo *operation = findOperationBySymbol(word);
            
            if (operation) {
                // fprintf(stderr, "[%s]\n", operation->symbol); // TODO
                node<diffNode> *nodePointer = OPERATION_NODE_(NULL, NULL, operation->name);
                isOperation                 = true;

                if (diff->tokens) {
                    diff->tokens->tokenArray[diff->tokens->count++] = {.tokenPointer = symbol - letterNumber - 1,
                                                                       .type         = OPERATION_NODE,
                                                                       .nextToken    = NULL};
                    // fprintf(stderr, "tokenArray[%lu] = %s\n", diff->tokens->count - 1, diff->tokens->tokenArray[diff->tokens->count - 1].tokenPointer); // TODO
                    if (diff->tokens->count - 1 > 0) {
                        diff->tokens->tokenArray[diff->tokens->count - 1].nextToken = 
                       &diff->tokens->tokenArray[diff->tokens->count];
                    }
                } // TODO FUNCTION CREATE TOKEN?

                break;
            }
            
            letterNumber++;

            if ((isalpha(*symbol) && ispunct(*(symbol - 1))) || (ispunct(*symbol) && isalpha(*(symbol - 1)))) {
                break;
            }
        }
        
        if (isOperation) {
            continue;
        }
        
              Variable  initWord = {.variableName = word};
        const Variable *findWord = findWordInTable(diff, &initWord);
        
        if (!findWord) {
            Variable newVariable = {.variableName = (char *)calloc(MAX_WORD_LENGTH + 1, sizeof(char)), .variableValue = NAN, .nextVariable = NULL};
            
            
            if (!newVariable.variableName) {
                return VARIABLE_TABLE_ERROR;
            }

            strncpy(newVariable.variableName, word, MAX_WORD_LENGTH);
            
            if (diff->variables) {
                
                diff->variables->variableArray[diff->variables->variableCount++] = newVariable;
                
                if (diff->variables->variableCount - 1 > 0) {
                    diff->variables->variableArray[diff->variables->variableCount - 1].nextVariable = &newVariable;
                }
            }

            findWord = &diff->variables->variableArray[diff->variables->variableCount - 1];

        }

        if (diff->tokens) {
            diff->tokens->tokenArray[diff->tokens->count++] = {.tokenPointer = symbol - 1,
                                                               .type         = VARIABLE_NODE,
                                                               .nextToken    = NULL};
            // fprintf(stderr, "tokenArray[%lu] = %s\n", diff->tokens->count - 1, diff->tokens->tokenArray[diff->tokens->count - 1].tokenPointer); // TODO
            if (diff->tokens->count - 1 > 0) {
                diff->tokens->tokenArray[diff->tokens->count - 1].nextToken = 
               &diff->tokens->tokenArray[diff->tokens->count];
            }
        } // TODO FUNCTION CREATE TOKEN?

        node<diffNode> *nodePointer = VARIABLE_(*findWord->variableName);
        // fprintf(stderr, "[%c]\n", nodePointer->data.nodeValue.varIndex); // TODO
    }

    free(word);

    node<diffNode> *nodePointer = OPERATION_NODE_(NULL, NULL, EMPTY_OPERATOR);

    return NO_PARSE_ERRORS;
}
