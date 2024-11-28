#include "differentiator.h"

#define OPERATOR(NAME, SYMBOL, PRIORITY, ...) {.name = NAME, .symbol = SYMBOL, .priority = PRIORITY},
const operationInfo operations[] = {
    #include "diffOperations.def"
};
#undef OPERATOR



