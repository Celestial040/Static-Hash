#ifndef GROUPING_BUFFER_H
#define GROUPING_BUFFER_H

#include "status.h"

typedef struct Grouping_Buffer {
    unsigned char *count;
    unsigned char *start;
    unsigned short capacity;
    unsigned short size;
} Grouping_Buffer;

typedef struct Grouping_Buffer_Status {
    Status status;
    Grouping_Buffer buffer;
}Grouping_Buffer_Status;

Grouping_Buffer_Status allocate_grouping_buffer(const unsigned short length);
Status insert_grouping_buffer(Grouping_Buffer *grouping, const unsigned char count);
void free_grouping_buffer(Grouping_Buffer *grouping);

#endif
