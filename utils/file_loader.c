#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "status.h"
#include "file_loader.h"

Status load_file_to_memory(const char *filename, FileString *output) {
    FILE *file_pointer;
    char *buffer;
    long file_len;
    size_t length;

    file_pointer = fopen(filename, "rb");
    if (file_pointer == NULL) {
        return FILE_NOT_FOUND;
    }

    fseek(file_pointer, 0, SEEK_END);
    file_len = ftell(file_pointer);
    fseek(file_pointer, 0, SEEK_SET);

    if (file_len < 0) {
        fclose(file_pointer);
        return READ_ERROR;
    }

    length = (size_t) file_len;

    buffer = (char*) malloc(sizeof(char) * (length+1));
    if (buffer == NULL) {
        fclose(file_pointer);
        return ALLOCATION_ERROR;
    }

    fread(buffer, 1, length, file_pointer);
    buffer[length] = '\0';

    output->start = buffer;
    output->length = length;

    fclose(file_pointer);

    return NO_ERROR;
}

Status free_file_string(FileString *filestring) {
    if (filestring->start || filestring->length) {
        return NULL_POINTER;
    }
    free(filestring->start);
    filestring->length = 0;
    return NO_ERROR;
}
