#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "differentiator.h"
#include "latex.h"
#include "customWarning.h"
#include "colorPrint.h"
#include "DSL.h"

static const size_t MAX_EXPRESSION_LENGTH = 200;

FILE *writeToLatex(Differentiator *diff, node<diffNode> *rootNode) {
    customWarning(diff     != NULL, NULL);
    customWarning(rootNode != NULL, NULL);

    char *fileName = (char *)calloc(MAX_FILE_NAME_SIZE, sizeof(char));
    customWarning(fileName != NULL, NULL);

    strncpy(fileName, strchr(diff->diffTree.infoData->htmlDumpPath, '/') + 1,
                      strchr(diff->diffTree.infoData->htmlDumpPath, '\0') -
                      strchr(diff->diffTree.infoData->htmlDumpPath, '/') - strlen(".html") - 1);

    snprintf(diff->latexPath, MAX_FILE_NAME_SIZE, "LaTex/%s.tex", fileName);
    
    char *buffer = (char *)calloc(MAX_CMD_BUFFER_SIZE, sizeof(char));
    customWarning(buffer != NULL, NULL);

    snprintf(buffer, MAX_CMD_BUFFER_SIZE, "touch %s", diff->latexPath);
    system(buffer);

    FREE_(fileName);
    FREE_(buffer);

    FILE *latexStream = fopen(diff->latexPath, "a"); // TODO CHECK penis size 8===========D------*
                                                     //                        |<-------->|
                                                     //                             x

    writeHeader(diff, latexStream);

    return latexStream;
}

latexError internalWriteToLatex(Differentiator *diff, node<diffNode> *rootNode, FILE *latexStream) {
    customWarning(diff        != NULL, (latexError) DIFF_NULL_PTR);
    customWarning(rootNode    != NULL, (latexError) NODE_NULL_PTR);
    customWarning(latexStream != NULL,              LATEX_STREAM_ERROR);

    if (!rootNode) {
        return NO_LATEX_ERRORS;
    }

    if (rootNode->data.type != OPERATION_NODE) {
        writeValue(rootNode, latexStream);
    }

    #define OPERATOR(NAME, SYMBOL, PRIORITY, EVAL_FUNCTION, DIFF_FUNCTION, LEFT_ZERO_SIMPLIFICATION,             \
                                                                           RIGHT_ZERO_SIMPLIFICATION,            \
                                                                           LEFT_ONE_SIMPLIFICATION,              \
                                                                           RIGHT_ONE_SIMPLIFICATION,             \
                                                                           LEFT_MINUS_ONE_SIMPLIFICATION,        \
                                                                           RIGHT_MINUS_ONE_SIMPLIFICATION,       \
                                                                           LATEX_FUNCTION)                       \
        if (rootNode->data.type == OPERATION_NODE && rootNode->data.nodeValue.op == NAME) {                      \
            LATEX_FUNCTION;                                                                                      \
        }                               

    #include "diffOperations.def"

    return NO_LATEX_ERRORS;
}

latexError writeValue(node<diffNode> *rootNode, FILE *latexStream) {
    customWarning(rootNode    != NULL, (latexError) NODE_NULL_PTR);
    customWarning(latexStream != NULL,              LATEX_STREAM_ERROR);

    switch (rootNode->data.type) {
        case OPERATION_NODE:
            {
                break;
            }
        
        case VARIABLE_NODE:
            {
                fprintf(latexStream, "%c", rootNode->data.nodeValue.varIndex);
                break;
            }
        
        case NUMERICAL_NODE:
            {
                fprintf(latexStream, "%lg", rootNode->data.nodeValue.value);
                break;
            }

        default:
            {
                customPrint(red, bold, bgDefault, "Error by writing value: %s:%d", __PRETTY_FUNCTION__, __LINE__);
                customWarning(NULL, WRITE_VALUE_ERROR);
            }
    }

    return NO_LATEX_ERRORS;
}

latexError writeHeader(Differentiator *diff, FILE *latexStream) {
    customWarning(diff        != NULL, (latexError) DIFF_NULL_PTR);
    customWarning(latexStream != NULL,              LATEX_STREAM_ERROR);

    fprintf(latexStream, "%s", head);

    fprintf(latexStream, "$$ f(x) = ");
    internalWriteToLatex(diff, diff->diffTree.root, latexStream);
    fprintf(latexStream, "$$\\ \\hline \\\n");

    fprintf(latexStream, "%s", endHead);

    return NO_LATEX_ERRORS;
}

latexError writeEnding(Differentiator *diff, FILE *latexStream) {
    customWarning(diff        != NULL, (latexError) DIFF_NULL_PTR);
    customWarning(latexStream != NULL,              LATEX_STREAM_ERROR);
    
    fprintf(latexStream, "%s", end);

    fclose(latexStream);

    return NO_LATEX_ERRORS;
}

latexError writePhrase(const char **array, FILE *latexStream, const size_t arraySize) {
    customWarning(array       != NULL, PHRASE_ERROR);
    customWarning(latexStream != NULL, LATEX_STREAM_ERROR);

    static size_t seed = 0;
    srand(seed);

    size_t phrase = (size_t) rand() % arraySize;
    seed          = phrase;

    customPrint(red, bold, bgDefault, "%lu\n", phrase);

    fprintf(latexStream, "\\section*{%s}\n", array[(size_t) rand() % arraySize]);

    return NO_LATEX_ERRORS;
}
