#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "status.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct FileString {
    char *start;
    size_t length;
} FileString;

typedef struct OptionalFileString {
    Status status;
    FileString filestring;
}OptionalFileString;

OptionalFileString load_file_to_memory(const char *filename);

#endif
