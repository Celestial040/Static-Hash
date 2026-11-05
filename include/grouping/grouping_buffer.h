#ifndef GROUPING_BUFFER_H
#define GROUPING_BUFFER_H

#include "status.h"

typedef struct Grouping_Buffer {
    unsigned char *grouping_count;
    unsigned char *grouping_start_index;
    unsigned short length;
} Grouping_Buffer;

typedef struct Grouping_Buffer_Status {
    Status status;
    Grouping_Buffer buffer;
}Grouping_Buffer_Status;

Grouping_Buffer_Status allocate_grouping_buffer(const unsigned short length);
Grouping_Buffer move_grouping_buffer(Grouping_Buffer src_buffer);

#endif
