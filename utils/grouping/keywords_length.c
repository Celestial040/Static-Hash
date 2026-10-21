#include "status.h"
#include "parser.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



Status grouping_by_keywords_length(KeywordData *data, unsigned char *grouping_start) {
    unsigned char i = 0;

    char *keywords;
    unsigned char *keywords_length;
    unsigned short *values_pair;
    unsigned char current_keyword_length = 0;
    unsigned char target_index = 0;

    keywords = calloc(data->keyword_count,sizeof(char));
    if (keywords == NULL) {
        return ALLOCATION_ERROR;
    }

    keywords_length = malloc(sizeof(unsigned char) * data->keyword_count);
    if (keywords_length == NULL) {
        return ALLOCATION_ERROR;
    }

    values_pair = malloc(sizeof(unsigned char) * data->keyword_count);
    if (values_pair == NULL) {
        return ALLOCATION_ERROR;
    }


    for (i = 0; i < data->keyword_count; i++) {
        current_keyword_length = data->keywords_length[i];
        target_index = grouping_start[current_keyword_length];

        memcpy(keywords + target_index * data->longest_keyword_len, data->keywords + i * data->longest_keyword_len , data->keywords_length[i]);
        keywords_length[target_index] = data->keywords_length[i];
        values_pair[target_index] = data->values_pair[i];

        grouping_start[current_keyword_length]++;
    }

    free(data->keywords);
    data->keywords = keywords;
    free(data->keywords_length);
    data->keywords_length = keywords_length;
    free(data->values_pair);
    data->values_pair = values_pair;

    return NO_ERROR;
}
