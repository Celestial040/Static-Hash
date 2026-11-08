#ifndef GROUPING_SELECTED_COLUMN_H
#define GROUPING_SELECTED_COLUMN_H

#include "status.h"
#include "keyword_data.h"

typedef struct Internal_Grouping_By_Selected_Column {
    Status status;
    Partial_Keyword_Data data;
}Internal_Grouping_By_Selected_Column;

Internal_Grouping_By_Selected_Column grouping_by_selected_column(
    const Keyword_Data data,
    const unsigned char selected_column,
    const unsigned char *uppercase_start,
    const unsigned char *lowercase_start,
    const unsigned char start,
    const unsigned char length
);

#endif
