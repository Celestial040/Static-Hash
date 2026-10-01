#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "status.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct FileString {
    char *start;
    size_t length;
} FileString;

Status load_file_to_memory(const char *filename, FileString *output);

#endif
