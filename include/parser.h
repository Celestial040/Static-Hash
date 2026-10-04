#ifndef PARSER_H
#define PARSER_H

#include "file_loader.h"
#include "status.h"
#include <stdlib.h>


typedef struct Keyword {
    size_t start;
    size_t len;
} Keyword ;

Status parser_start(const FileString *filestring);

#endif
