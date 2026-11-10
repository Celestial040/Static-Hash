#ifndef KEYWORD_DATA_H
#define KEYWORD_DATA_H

#include "status.h"

typedef struct Keyword_Data {
    char *keywords;
    unsigned char *keywords_length;
    unsigned short *values_pair;
    unsigned char keyword_count;
    unsigned char longest_keyword_len;
    unsigned char shortest_keyword_len;
} Keyword_Data;

typedef struct Keyword_Data_Status {
    Status status;
    Keyword_Data data;
}Keyword_Data_Status;

typedef struct Partial_Keyword_Data {
    char *keywords;
    unsigned char *keywords_length;
    unsigned short *values_pair;
} Partial_Keyword_Data;

typedef struct Partial_Keyword_Data_Status {
    Status status;
    Partial_Keyword_Data data;
}Partial_Keyword_Data_Status;

void print_keyword_data(const Keyword_Data data);
void print_keyword_data_details(const Keyword_Data data);
void print_partial_keyword_data_details(
    const Partial_Keyword_Data data,
    const unsigned char count,
    const unsigned char longest_keyword_len
);
void print_partial_keyword_data(
    const Partial_Keyword_Data data,
    const unsigned char count,
    const unsigned char longest_keyword_len
);

Keyword_Data_Status allocate_keyword_data(
    const unsigned char keyword_count,
    const unsigned char shortest_keyword_len,
    const unsigned char longest_keyword_len
);

Keyword_Data_Status copy_keyword_data(const Keyword_Data data);

Partial_Keyword_Data_Status copy_partial_keyword_data(
    const Keyword_Data data,
    const unsigned char row_start,
    const unsigned char row_count
);

void free_keyword_data(Keyword_Data *data);
void free_partial_keyword_data(Partial_Keyword_Data *data);


#endif
