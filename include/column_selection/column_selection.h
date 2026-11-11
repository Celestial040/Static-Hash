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
    Unsigned_Char_Vector best_columns;
    Status status;
} Keyword_Data__Best_Columns;

typedef struct Keyword_Data__Grouping__Best_Columns {
    Keyword_Data data;
    Grouping_Buffer grouping;
    Unsigned_Char_Vector best_columns;
    Status status;
} Keyword_Data__Grouping__Best_Columns;

typedef struct Keyword_Data__Grouping__Selected_Column {
    Keyword_Data data;
    Grouping_Buffer grouping;
    unsigned char selected_column;
    Status status;
} Keyword_Data__Grouping__Selected_Column;

Keyword_Data__Best_Columns column_selection(Keyword_Data data);


#endif
