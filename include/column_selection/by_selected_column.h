#ifndef COLUMN_SELECTION_BY_SELECTED_COLUMN_H
#define COLUMN_SELECTION_BY_SELECTED_COLUMN_H

#include "status.h"
#include "keyword_data.h"
#include "grouping/grouping_buffer.h"
#include "column_selection/column_selection.h"

typedef struct Grouping_By_Selected_Column {
    Status status;
    Keyword_Data data;
    Grouping_Buffer grouping;
} Grouping_By_Selected_Column;

Grouping_By_Selected_Column row_grouping_based_on_selected_column(
    Column_Selection_Internal_Result *src_data
);


#endif
