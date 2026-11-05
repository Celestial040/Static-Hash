#ifndef COLUMN_SELECTION_BY_LENGTH_H
#define COLUMN_SELECTION_BY_LENGTH_H

#include "status.h"
#include "keyword_data.h"
#include "grouping/grouping_buffer.h"

typedef struct Grouping_By_Length {
    Status status;
    Keyword_Data data;
    Grouping_Buffer grouping_buffer;
} Grouping_By_Length;

Grouping_By_Length row_grouping_by_length(const Keyword_Data data);

#endif
