#ifndef PARSER_H
#define PARSER_H

#include "file_loader.h"
#include "status.h"
#include <stdlib.h>


typedef struct Keyword {
    size_t start;
    size_t len;
} Keyword ;

typedef struct KeywordData {
    Keyword *keywords;
    unsigned char *keyword_column;
    unsigned short *values_pair;
    size_t keyword_count;
    unsigned char longest_keyword_len;
} KeywordData;


Status parser_start(const FileString *filestring);

#endif
