#ifndef COLUMN_SELECTION_H
#define COLUMN_SELECTION_H

#include "status.h"
#include "keyword_data.h"
#include "vector/unsigned_char.h"
#include "grouping/grouping_buffer.h"

typedef struct Keyword_Data__Grouping {
    Keyword_Data data;
    Grouping_Buffer grouping;
    Status status;
} Keyword_Data__Grouping;

typedef struct Keyword_Data__Best_Columns {
    Keyword_Data data;
    unsigned_char_Vector best_columns;
    Status status;
} Keyword_Data__Best_Columns;

typedef struct Keyword_Data__Grouping__Best_Columns {
    Keyword_Data data;
    Grouping_Buffer grouping;
    unsigned_char_Vector best_columns;
    Status status;
} Keyword_Data__Grouping__Best_Columns;

typedef struct Keyword_Data__Grouping__Selected_Column {
    Keyword_Data data;
    Grouping_Buffer grouping;
    unsigned char selected_column;
    Status status;
} Keyword_Data__Grouping__Selected_Column;

Keyword_Data__Best_Columns column_selection(Keyword_Data *src_data);

typedef struct Column_Selection_Internal_Result {
    Keyword_Data data;
    Grouping_Buffer grouping;
    Status status;
    unsigned char selected_column;
}Column_Selection_Internal_Result;



#endif
