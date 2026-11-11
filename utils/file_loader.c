
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "status.h"
#include "file_loader.h"

FileString load_file_to_memory(const char *filename) {
    FileString result = {NO_ERROR, NULL, 0};

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
    result.start = buffer;
    result.length = len;

    fclose(file_pointer);

    return result;
}

FileString move_filestring(FileString *filestring) {
    FileString result = {
        0, NULL, 0
    };

    result.status = filestring->status;
    result.start = filestring->start;
    result.length = filestring->length;

    filestring->status = 0;
    filestring->start = NULL;
    filestring->length = 0;

    return result;
}

void free_filestring(FileString *filestring) {
    free(filestring->start);
    filestring->length = 0;
    filestring->status = 0;
}
