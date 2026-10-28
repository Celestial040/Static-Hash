#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "status.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct FileString {
    Status status;
    char *start;
    size_t length;
} FileString;


FileString load_file_to_memory(const char *filename);

#endif
