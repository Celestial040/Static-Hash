#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "sort/median.h"

void swap_sort_base_on_weight(unsigned short *weight, unsigned char *keyword_index, unsigned short *values_pair, size_t index_1, size_t index_2) {
    unsigned char temp_keyword_index = 0;
    unsigned short temp_values_pair = 0;
    unsigned short temp_weight = 0;

    temp_keyword_index = keyword_index[index_1];
    keyword_index[index_1] = keyword_index[index_2];
    keyword_index[index_2] = temp_keyword_index;

    temp_values_pair = values_pair[index_1];
    values_pair[index_1] = values_pair[index_2];
    values_pair[index_2] = temp_values_pair;

    temp_weight = weight[index_1];
    weight[index_1] = weight[index_2];
    weight[index_2] = temp_weight;

}

void most_collide_internal_quicksort(unsigned short *weight, unsigned char *keyword_index, unsigned short *values_pair, size_t low, size_t high, unsigned short pivot) {
    size_t i = low;
    size_t j = high;
    unsigned short next_pivot = 0;

    if (low == high) {
        return;
    }

    if (high - low == 1) {
        if (weight[low] < weight[high]) {
            swap_sort_base_on_weight(weight, keyword_index, values_pair, low, high);
        }
        return;
    }

    while (1) {
        while (weight[i] > pivot) {
            i++;
        }

        while (weight[j] < pivot) {
            j--;
        }

        if (i<j) {

            swap_sort_base_on_weight(weight, keyword_index, values_pair, i, j);

            j--;
            i++;
        } else {
            break;
        }
    }

    next_pivot = median_of_three(weight[low], weight[(low + j) / 2], weight[j]);
    most_collide_internal_quicksort(weight, keyword_index, values_pair, low, j, next_pivot);
    next_pivot = median_of_three(weight[i], weight[(i + high) / 2], weight[high]);
    most_collide_internal_quicksort(weight, keyword_index, values_pair, i, high, next_pivot);
}

Status most_collide_keyword_sort(KeywordData *keyword_data, unsigned short *weight) {
    char *temp_keyword = NULL;
    char *temp_keyword_column = NULL;
    unsigned char *temp_keyword_index = NULL;
    unsigned short *temp_values_pair = NULL;
    unsigned short *temp_weight = NULL;
    unsigned short pivot = 0;
    size_t i, j = 0;

    temp_keyword = (char *) malloc(sizeof(char) * keyword_data->keyword_count);
    if (temp_keyword == NULL) {
        return ALLOCATION_ERROR;
    }

    temp_keyword_index = (unsigned char *) malloc(sizeof(unsigned char) * keyword_data->keyword_count);
    if (temp_keyword_index == NULL) {
        return ALLOCATION_ERROR;
    }

    temp_keyword_column = (char *) malloc(sizeof(char) * keyword_data->keyword_count * keyword_data->longest_keyword_len);
    if (temp_keyword_column == NULL) {
        return ALLOCATION_ERROR;
    }

    temp_values_pair = (unsigned short *) malloc(sizeof(unsigned short) * keyword_data->keyword_count);
    if (temp_values_pair == NULL) {
        return ALLOCATION_ERROR;
    }

    temp_weight = (unsigned short *) malloc(sizeof(unsigned short) * keyword_data->keyword_count);
    if (temp_weight == NULL) {
        return ALLOCATION_ERROR;
    }

    for (i = 0; i < keyword_data->keyword_count; i++) {
        temp_keyword_index[i] = (unsigned char) i;
    }

    memcpy(temp_keyword, keyword_data->keywords, sizeof(char) * keyword_data->keyword_count);
    memcpy(temp_values_pair, keyword_data->values_pair, sizeof(unsigned short) * keyword_data->keyword_count);
    memcpy(temp_weight, weight, sizeof(unsigned short) * keyword_data->keyword_count);

    pivot = median_of_three(temp_weight[0], temp_weight[(keyword_data->keyword_count-1) / 2], temp_weight[keyword_data->keyword_count-1]);
    most_collide_internal_quicksort(temp_weight, temp_keyword_index, temp_values_pair, 0, keyword_data->keyword_count-1, pivot);

    for (i = 0; i < keyword_data->keyword_count; i++) {
        for (j = 0; j < keyword_data->longest_keyword_len; j++) {
            temp_keyword[keyword_data->keyword_count * i + j] = keyword_data->keywords[keyword_data->keyword_count * temp_keyword_index[i] + j];
        }
    }
    for (i = 0; i < keyword_data->longest_keyword_len; i++) {
        for (j = 0; j < keyword_data->keyword_count; j++) {
            temp_keyword_column[i * keyword_data->longest_keyword_len + j] = temp_keyword[j * keyword_data->keyword_count + i];
        }
    }

    free(keyword_data->keywords);
    keyword_data->keywords = temp_keyword;
    free(keyword_data->values_pair);
    keyword_data->values_pair = keyword_data->values_pair;
    free(keyword_data->keyword_column);
    keyword_data->keyword_column = temp_keyword_column;

    return NO_ERROR;
}
