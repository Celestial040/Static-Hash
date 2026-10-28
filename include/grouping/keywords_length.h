#ifndef GROUPING_KEYWORD_LENGTH_H
#define GROUPING_KEYWORD_LENGTH_H

#include "status.h"
#include "parser.h"

typedef struct OptionalInternalGroupingByLength {
    Status status;
    KeywordData data;
} OptionalInternalGroupingByLength;

OptionalInternalGroupingByLength grouping_by_keywords_length(const KeywordData data, const unsigned char *grouping_start);

#endif
