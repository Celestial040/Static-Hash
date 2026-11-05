#ifndef COLUMN_PICK_H
#define COLUMN_PICK_H

#include "keyword_data.h"

unsigned char column_pick_based_on_length(
    const Keyword_Data data,
    const unsigned char *group_count,
    const unsigned char *group_start
);

#endif
