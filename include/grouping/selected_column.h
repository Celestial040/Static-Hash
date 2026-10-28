#ifndef GROUPING_SELECTED_COLUMN_H
#define GROUPING_SELECTED_COLUMN_H

#include "status.h"
#include "parser.h"

typedef struct Internal_Grouping_BySelectedColumn {
    Status status;
    char *keywords;
    unsigned char *keywords_length;
    unsigned short *values_pair;
}Internal_Grouping_BySelectedColumn;

Internal_Grouping_BySelectedColumn grouping_by_selected_column(
    const KeywordData data,
    const unsigned char selected_column,
    const unsigned char *uppercase_start,
    const unsigned char *lowercase_start,
    const unsigned char start,
    const unsigned char length
);

#endif
