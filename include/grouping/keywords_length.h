#ifndef GROUPING_KEYWORD_LENGTH_H
#define GROUPING_KEYWORD_LENGTH_H

#include "status.h"
#include "parser.h"

typedef struct Internal_Grouping_ByLength {
    Status status;
    KeywordData data;
} Internal_Grouping_ByLength;

Internal_Grouping_ByLength grouping_by_keywords_length(const KeywordData data, const unsigned char *grouping_start);

#endif
