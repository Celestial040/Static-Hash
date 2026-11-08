#ifndef GROUPING_KEYWORD_LENGTH_H
#define GROUPING_KEYWORD_LENGTH_H

#include "status.h"
#include "keyword_data.h"

typedef struct Internal_Grouping_By_Length {
    Status status;
    Keyword_Data data;
} Internal_Grouping_By_Length;

Internal_Grouping_By_Length grouping_by_keywords_length(
    const Keyword_Data data,
    const unsigned char *grouping_start
);

#endif
