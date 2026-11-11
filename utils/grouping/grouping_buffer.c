#include "grouping/grouping_buffer.h"
#include <stdlib.h>

Grouping_Buffer_Status allocate_grouping_buffer(const unsigned short capacity) {
    Grouping_Buffer_Status result = {
        NO_ERROR, {NULL, NULL, 0, 0}
    };

    Grouping_Buffer grouping = {
        NULL, NULL, 0, 0
    };

    grouping.capacity = capacity;
    grouping.size = 0;

    grouping.count = (unsigned char *) calloc(capacity, sizeof(unsigned char));
    if (grouping.count == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    grouping.start = (unsigned char *) calloc(capacity, sizeof(unsigned char));
    if (grouping.start == NULL) {
        free(grouping.count);

        result.status = ALLOCATION_ERROR;
        return result;
    }

    result.buffer = grouping;
    return result;
}

Status insert_grouping_buffer(Grouping_Buffer *grouping, const unsigned char count) {
    grouping->count[grouping->size] = count;
    if (grouping->size > 0) {
        grouping->start[grouping->size] += grouping->start[grouping->size-1];
        grouping->start[grouping->size] += grouping->count[grouping->size-1];
    }
    grouping->size++;
    return NO_ERROR;
}

Grouping_Buffer move_grouping_buffer(Grouping_Buffer *grouping) {
    Grouping_Buffer result = {
        NULL, NULL, 0, 0
    };

    result.start = grouping->start;
    result.count = grouping->count;
    result.size = grouping->size;
    result.capacity = grouping->capacity;

    grouping->start = NULL;
    grouping->count = NULL;
    grouping->size = 0;
    grouping->capacity = 0;

    return result;
}



void free_grouping_buffer(Grouping_Buffer *grouping) {
    free(grouping->start);
    free(grouping->count);

    grouping->start = NULL;
    grouping->count = NULL;
    grouping->size = 0;
    grouping->capacity = 0;

}
