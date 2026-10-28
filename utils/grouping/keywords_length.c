#include "status.h"
#include "parser.h"
#include "grouping/keywords_length.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



OptionalInternalGroupingByLength grouping_by_keywords_length(const KeywordData data, const unsigned char *grouping_start) {
    OptionalInternalGroupingByLength result = {NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}};

    char *keywords = NULL;
    unsigned char *keywords_length = NULL;
    unsigned short *values_pair = NULL;
    unsigned char *grouping_start_copy = NULL;

    unsigned char current_keyword_length = 0;
    unsigned char target_index = 0;
    unsigned char i = 0;

    keywords = (char *) calloc(data.keyword_count,sizeof(char));
    if (keywords == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    keywords_length = (unsigned char *) malloc(sizeof(unsigned char) * data.keyword_count);
    if (keywords_length == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    values_pair = (unsigned short *) malloc(sizeof(unsigned char) * data.keyword_count);
    if (values_pair == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    grouping_start_copy = (unsigned char *) calloc(data.longest_keyword_len + 1, sizeof(unsigned char));
    if (grouping_start_copy == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    memcpy(grouping_start_copy, grouping_start, data.longest_keyword_len + 1);

    for (i = 0; i < data.keyword_count; i++) {
        current_keyword_length = data.keywords_length[i];
        target_index = grouping_start_copy[current_keyword_length];

        memcpy(keywords + target_index * data.longest_keyword_len, data.keywords + i * data.longest_keyword_len , data.keywords_length[i]);
        keywords_length[target_index] = data.keywords_length[i];
        values_pair[target_index] = data.values_pair[i];

        grouping_start_copy[current_keyword_length]++;
    }

    free(grouping_start_copy);

    result.data.keyword_count = data.keyword_count;
    result.data.longest_keyword_len = data.longest_keyword_len;
    result.data.shortest_keyword_len = data.shortest_keyword_len;
    result.data.keywords = keywords;
    result.data.keywords_length = keywords_length;
    result.data.values_pair = values_pair;

    return result;
}
