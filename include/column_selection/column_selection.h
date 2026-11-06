#ifndef COLUMN_SELECTION_H
#define COLUMN_SELECTION_H

#include "status.h"
#include "keyword_data.h"
#include "vector/unsigned_char.h"

typedef struct Column_Selection {
    Status status;
    Keyword_Data data;
    unsigned_char_Vector best_column;
} Column_Selection;

Column_Selection column_selection_start(Keyword_Data src_data);

#endif
