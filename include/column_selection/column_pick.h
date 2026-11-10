#ifndef COLUMN_PICK_H
#define COLUMN_PICK_H

#include "keyword_data.h"
#include "grouping/grouping_buffer.h"
#include "vector/unsigned_char.h"

unsigned char column_pick_based_on_length(
    const Keyword_Data data,
    const Grouping_Buffer buffer
);

unsigned char column_pick_based_on_previous_selected_column(
    const Keyword_Data data,
    const Grouping_Buffer grouping,
    const unsigned_char_Vector column_to_avoid
);

#endif
