#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "status.h"
#include "column_selection/column_selection.h"
#include "grouping/keywords_length.h"

void generating_grouping_buffer_base_on_keywords_length(const Keyword_Data data, Grouping_Buffer grouping) {
    unsigned char i = 0;
    unsigned char prev = 0;
    unsigned char offset = 0;


    for (i = 0; i < data.keyword_count; i++) {
        grouping.count[data.keywords_length[i]]++;
    }

    for (i = 0; i < data.longest_keyword_len + 1; i++) {
        prev = grouping.count[i];
        grouping.start[i] = offset;
        offset += prev;
    }
}

Keyword_Data__Grouping row_grouping_by_length(Keyword_Data data) {
    Keyword_Data__Grouping result = {
        {NULL, NULL, NULL, 0, 0 ,0}, {NULL, NULL, 0, 0}, NO_ERROR
    };

    Grouping_Buffer_Status grouping = {
        NO_ERROR, {NULL, NULL, 0, 0}
    };

    Keyword_Data_Status internal_result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}
    };


    grouping = allocate_grouping_buffer(data.longest_keyword_len + 1);
    if (grouping.status != NO_ERROR) {
        result.status = grouping.status;
        return result;
    }

    generating_grouping_buffer_base_on_keywords_length(data, grouping.buffer);

    internal_result = grouping_by_keywords_length(
        data, grouping.buffer.start
    );

    if (internal_result.status != NO_ERROR) {
        result.status = internal_result.status;
        return result;
    }

    result.data = internal_result.data;
    result.grouping = grouping.buffer;
    free_keyword_data(&data);

    return result;
}
