#ifndef RECURSIVE_DESCENT_PARSER_H_
#define RECURSIVE_DESCENT_PARSER_H_

#include "differentiator.h"
#include "colorPrint.h"

#define SYNTAX_ERROR(function) {                                        \
    customPrint(red, bold, bgDefault, "Syntax Error in %s", #function); \
    exit(-1);                                                           \
}

// FUNCTION PROTOTYPES //

// FUNCTION PROTOTYPES //

#endif // RECURSIVE_DESCENT_PARSER_H_
