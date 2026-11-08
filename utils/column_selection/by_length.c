#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "status.h"
#include "column_selection/by_length.h"
#include "grouping/keywords_length.h"

Grouping_Buffer generating_grouping_buffer_base_on_keywords_length(const Keyword_Data data, Grouping_Buffer src_grouping);

Grouping_By_Length row_grouping_by_length(Keyword_Data src_data) {
    Grouping_By_Length result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}, {NULL, NULL, 0, 0}
    };

    Grouping_Buffer_Status grouping = {
        NO_ERROR, {NULL, NULL, 0, 0}
    };

    Internal_Grouping_By_Length internal_result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}
    };

    Keyword_Data data = {
        NULL, NULL, NULL, 0, 0, 0
    };

    data = move_keyword_data(&src_data);

    grouping = allocate_grouping_buffer(data.longest_keyword_len + 1);
    if (grouping.status != NO_ERROR) {
        result.status = grouping.status;
        return result;
    }

    grouping.buffer = generating_grouping_buffer_base_on_keywords_length(data, grouping.buffer);

    internal_result = grouping_by_keywords_length(
        data, grouping.buffer.start
    );

    if (internal_result.status != NO_ERROR) {
        result.status = internal_result.status;
        return result;
    }

    result.data = internal_result.data;
    result.grouping = grouping.buffer;

    return result;
}

Grouping_Buffer generating_grouping_buffer_base_on_keywords_length(const Keyword_Data data, Grouping_Buffer src_grouping) {
    Grouping_Buffer grouping = {NULL, NULL, 0, 0};
    unsigned char i = 0;
    unsigned char prev = 0;
    unsigned char offset = 0;

    grouping = move_grouping_buffer(&src_grouping);

    for (i = 0; i < data.keyword_count; i++) {
        grouping.count[data.keywords_length[i]]++;
    }

    for (i = 0; i < data.longest_keyword_len + 1; i++) {
        prev = grouping.count[i];
        grouping.start[i] = offset;
        offset += prev;
    }

    return grouping;
}
