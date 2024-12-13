#ifndef LEXER_H_
#define LEXER_H_

#include "differentiator.h"

const size_t MAX_WORD_LENGTH = 10;

enum parseError {
    NO_PARSE_ERRORS      = 0,
    DIFF_PTR_ERROR       = 1,
    VARIABLE_TABLE_ERROR = 2
};

// FUNCTION PROTOTYPES //
parseError expressionLexer(Differentiator *diff);
// FUNCTION PROTOTYPES //

#endif // LEXER_H_
