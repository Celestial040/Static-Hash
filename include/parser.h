#ifndef PARSER_H
#define PARSER_H

#include "file_loader.h"
#include "status.h"


typedef struct KeywordData {
    char *keywords;
    char *keyword_column;
    unsigned short *values_pair;
    unsigned char keyword_count;
    unsigned char longest_keyword_len;
} KeywordData;


Status parser_start(const FileString *filestring);

#endif
