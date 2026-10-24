#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "status.h"
#include "file_loader.h"

OptionalFileString load_file_to_memory(const char *filename) {
    OptionalFileString result = {NO_ERROR, {NULL, 0}};

    FILE *file_pointer = NULL;
    char *buffer = NULL;
    long file_length = 0;
    size_t len = 0;

    file_pointer = fopen(filename, "rb");
    if (file_pointer == NULL) {
        result.status = FILE_NOT_FOUND;
        return result;
    }

    fseek(file_pointer, 0, SEEK_END);
    file_length = ftell(file_pointer);
    fseek(file_pointer, 0, SEEK_SET);

    if (file_length < 0) {
        fclose(file_pointer);

        result.status = READ_ERROR;
        return result;
    }

    len = (size_t) file_length;

    buffer = (char*) malloc(sizeof(char) * (len+1));
    if (buffer == NULL) {
        fclose(file_pointer);

        result.status = ALLOCATION_ERROR;
        return result;
    }

    fread(buffer, 1, len, file_pointer);
    buffer[len] = '\0';

    result.status = NO_ERROR;
    result.filestring.start = buffer;
    result.filestring.length = len;

    fclose(file_pointer);

    return result;
}

Status free_file_string(FileString *filestring) {
    if (filestring->start || filestring->length) {
        return NULL_POINTER;
    }
    free(filestring->start);
    filestring->length = 0;
    return NO_ERROR;
}
