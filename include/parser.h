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
    unsigned char shortest_keyword_len;
} KeywordData;

typedef struct Parser {
    Status status;
}Parser;


Parser parser_start(const FileString filestring, const char *filename);
void print_keyword_data(const KeywordData data);
void print_keyword_data_details(const KeywordData data);

#endif
