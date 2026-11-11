#ifndef GROUPING_SELECTED_COLUMN_H
#define GROUPING_SELECTED_COLUMN_H

#include "keyword_data.h"

Partial_Keyword_Data_Status grouping_by_selected_column(
    const Keyword_Data data,
    const unsigned char selected_column,
    const unsigned char *uppercase_start,
    const unsigned char *lowercase_start,
    const unsigned char start,
    const unsigned char length
);

#endif
