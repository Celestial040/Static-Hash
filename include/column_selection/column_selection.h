#ifndef COLUMN_SELECTION_H
#define COLUMN_SELECTION_H

#include "status.h"
#include "keyword_data.h"
#include "vector/unsigned_char.h"
#include "grouping/grouping_buffer.h"

typedef struct Column_Selection {
    Status status;
    Keyword_Data data;
    unsigned_char_Vector best_column;
} Column_Selection;


typedef struct Column_Selection_Internal_Result {
    Keyword_Data data;
    Grouping_Buffer grouping;
    Status status;
    unsigned char selected_column;
}Column_Selection_Internal_Result;

Column_Selection column_selection_start(Keyword_Data *src_data);

#endif
