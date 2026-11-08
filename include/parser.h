#ifndef PARSER_H
#define PARSER_H

#include "file_loader.h"
#include "keyword_data.h"
#include "status.h"

typedef struct Parser {
    Status status;
    Keyword_Data data;
}Parser;


Parser parser_start(const FileString filestring, const char *filename);
#endif
