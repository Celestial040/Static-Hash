#include "status.h"
#include "grouping/keywords_length.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Internal_Grouping_By_Length grouping_by_keywords_length(
    Keyword_Data data,
    const unsigned char *grouping_start
) {
    Internal_Grouping_By_Length result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 , 0}
    };

    Keyword_Data_Status keyword_data_copy = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0, 0}
    };

    unsigned char *grouping_start_copy = NULL;

    unsigned char current_keyword_length = 0;
    unsigned char target_index = 0;
    unsigned char i = 0;

    keyword_data_copy = allocate_keyword_data(
        data.keyword_count,
        data.shortest_keyword_len,
        data.longest_keyword_len
    );

    if (keyword_data_copy.status != NO_ERROR) {
        result.status = keyword_data_copy.status;
        return result;
    }

    grouping_start_copy = (unsigned char *) malloc(
        sizeof(unsigned char) * data.longest_keyword_len + 1
    );

    if (grouping_start_copy == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    memcpy(
        grouping_start_copy,
        grouping_start,
        data.longest_keyword_len + 1
    );

    for (i = 0; i < data.keyword_count; i++) {
        current_keyword_length = data.keywords_length[i];
        target_index = grouping_start_copy[current_keyword_length];

        memcpy(
            keyword_data_copy.data.keywords + target_index * data.longest_keyword_len,
            data.keywords + i * data.longest_keyword_len,
            data.keywords_length[i]
        );

        keyword_data_copy.data.keywords_length[target_index] = data.keywords_length[i];
        keyword_data_copy.data.values_pair[target_index] = data.values_pair[i];

        grouping_start_copy[current_keyword_length]++;
    }

    free(grouping_start_copy);
    free_keyword_data(&data);

    result.data = keyword_data_copy.data;
    return result;
}
