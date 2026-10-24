#ifndef PARSER_H
#define PARSER_H

#include "file_loader.h"
#include "status.h"


typedef struct KeywordData {
    char *keywords;
    unsigned char *keywords_length;
    unsigned short *values_pair;
    unsigned char keyword_count;
    unsigned char longest_keyword_len;
} KeywordData;


Status parser_start(const FileString *filestring);
void print_keyword_data(KeywordData *data);
void print_keyword_data_details(KeywordData *data);

#endif
