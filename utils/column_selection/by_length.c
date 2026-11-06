#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "status.h"
#include "column_selection/by_length.h"
#include "grouping/keywords_length.h"

Grouping_Buffer filling_grouping_buffer_by_length(const Keyword_Data data, Grouping_Buffer src_buffer) {
    Grouping_Buffer buffer = {NULL, NULL, 0};
    unsigned char i = 0;
    unsigned char prev = 0;
    unsigned char offset = 0;

    buffer = move_grouping_buffer(src_buffer);

    for (i = 0; i < data.keyword_count; i++) {
        buffer.grouping_count[data.keywords_length[i]]++;
    }

    for (i = 0; i < data.longest_keyword_len + 1; i++) {
        prev = buffer.grouping_count[i];
        buffer.grouping_start_index[i] = offset;
        offset += prev;
    }

    return buffer;
}

Grouping_By_Length row_grouping_by_length(Keyword_Data src_data) {
    Grouping_By_Length result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}, {NULL, NULL, 0}
    };

    Grouping_Buffer_Status grouping_buffer = {
        NO_ERROR, {NULL, NULL, 0}
    };

    Internal_Grouping_By_Length internal_result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}
    };

    Keyword_Data data = {
        NULL, NULL, NULL, 0, 0, 0
    };

    data = move_keyword_data(src_data);

    grouping_buffer = allocate_grouping_buffer(data.longest_keyword_len + 1);
    if (grouping_buffer.status != NO_ERROR) {
        result.status = grouping_buffer.status;
        return result;
    }

    grouping_buffer.buffer = filling_grouping_buffer_by_length(data, grouping_buffer.buffer);

    internal_result = grouping_by_keywords_length(
        data, grouping_buffer.buffer.grouping_start_index
    );

    if (internal_result.status != NO_ERROR) {
        result.status = internal_result.status;
        return result;
    }

    result.data = internal_result.data;
    result.grouping_buffer = grouping_buffer.buffer;

    return result;
}
