#ifndef COLUMN_SELECTION_BY_SELECTED_COLUMN_H
#define COLUMN_SELECTION_BY_SELECTED_COLUMN_H

#include "status.h"
#include "keyword_data.h"
#include "grouping/grouping_buffer.h"

typedef struct Grouping_By_Selected_Column {
    Status status;
    Keyword_Data data;
    Grouping_Buffer grouping;
} Grouping_By_Selected_Column;

Grouping_By_Selected_Column row_grouping_based_on_selected_column(
    Keyword_Data *src_data,
    const Grouping_Buffer src_grouping,
    const unsigned char selected_column
);


#endif
