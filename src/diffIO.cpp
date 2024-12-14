#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "differentiator.h"
#include "diffIO.h"

diffError bufferInitialize(Differentiator *diff) {
    customWarning(diff != NULL, DIFF_NULL_PTR);

    diff->buffer           = (Buffer *)calloc(1,                 sizeof(Buffer));
    customWarning(diff->buffer           != NULL, BAD_ALLOCATION);

    diff->buffer->filePath = (char *)  calloc(MAX_FILEPATH_SIZE, sizeof(char));
    customWarning(diff->buffer->filePath != NULL, BAD_ALLOCATION);

    diff->buffer->text     = (char *)  calloc(MAX_BUFFER_SIZE,   sizeof(char));
    customWarning(diff->buffer->text     != NULL, BAD_ALLOCATION);

    return NO_DIFF_ERRORS;
}

diffError readFromFile(Differentiator *diff) {
    customWarning(diff != NULL, DIFF_NULL_PTR);

    struct stat fileData = {};
    stat(diff->buffer->filePath, &fileData);

    diff->buffer->textSize = (size_t)fileData.st_size;
    diff->buffer->text = (char *)calloc(diff->buffer->textSize, sizeof(char));
    int openFile = open(diff->buffer->filePath, O_RDONLY);
    customWarning(openFile != -1, (diffError) NO_SUCH_FILE);

    ssize_t sizeRead = read(openFile, diff->buffer->text, (size_t)diff->buffer->textSize);
    customWarning(sizeRead == diff->buffer->textSize, FILE_ERROR);

    close(openFile);

    return NO_DIFF_ERRORS;
}
