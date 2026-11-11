#ifndef COLUMN_SELECTION_BY_SELECTED_COLUMN_H
#define COLUMN_SELECTION_BY_SELECTED_COLUMN_H

#include "keyword_data.h"
#include "grouping/grouping_buffer.h"
#include "column_selection/column_selection.h"

Keyword_Data__Grouping row_grouping_based_on_selected_column(
    Keyword_Data data,
    Grouping_Buffer grouping,
    unsigned char selected_column,
    unsigned short grouping_start,
    unsigned short grouping_count
);


#endif
