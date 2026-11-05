#include "grouping/grouping_buffer.h"
#include <stdlib.h>

Grouping_Buffer_Status allocate_grouping_buffer(const unsigned short length) {
    Grouping_Buffer_Status result = {
        NO_ERROR, {NULL, NULL, 0}
    };

    Grouping_Buffer grouping_buffer = {
        NULL, NULL, 0
    };

    grouping_buffer.length = length;

    grouping_buffer.grouping_count = (unsigned char *) calloc(length, sizeof(unsigned char));
    if (grouping_buffer.grouping_count == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    grouping_buffer.grouping_start_index = (unsigned char *) calloc(length, sizeof(unsigned char));
    if (grouping_buffer.grouping_start_index == NULL) {
        free(grouping_buffer.grouping_count);

        result.status = ALLOCATION_ERROR;
        return result;
    }

    result.buffer = grouping_buffer;
    return result;
}
