#ifndef COLUMN_PICK_H
#define COLUMN_PICK_H

#include "keyword_data.h"
#include "grouping/grouping_buffer.h"

unsigned char column_pick_based_on_length(
    const Keyword_Data data,
    const Grouping_Buffer buffer
);

#endif
