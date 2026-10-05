#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


unsigned short median_of_three(unsigned short a, unsigned short b, unsigned short c) {
    if ((c > a && a > b) || (b > a && a > c)) {
        return a;
    }
    if ((a > b && b > c) || (c > b && b > a)) {
        return b;
    }
    if ((a > c && c > b) || (b > c && c > a)) {
        return c;
    }
    if ((a < b && b == c) || (a > b && b == c)) {
        return b;
    }
    if ((a < b && a == c) || (a > b && a == c)) {
        return a;
    }
    return b;
}

void swap_sort_base_on_weight(unsigned short *weight, Keyword *keyword, unsigned short *values_pair, size_t index_1, size_t index_2) {
    Keyword temp_keyword = {0,0};
    unsigned short temp_values_pair = 0;
    unsigned short temp_weight = 0;

    temp_keyword = keyword[index_1];
    keyword[index_1] = keyword[index_2];
    keyword[index_2] = temp_keyword;

    temp_values_pair = values_pair[index_1];
    values_pair[index_1] = values_pair[index_2];
    values_pair[index_2] = temp_values_pair;

    temp_weight = weight[index_1];
    weight[index_1] = weight[index_2];
    weight[index_2] = temp_weight;

}

void internal_quicksort(unsigned short *weight, Keyword *keyword, unsigned short *values_pair, size_t low, size_t high, unsigned short pivot) {
    size_t i = low;
    size_t j = high;
    unsigned short next_pivot = 0;

    if (low == high) {
        return;
    }

    if (high - low == 1) {
        if (weight[low] < weight[high]) {
            swap_sort_base_on_weight(weight, keyword, values_pair, low, high);
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

            swap_sort_base_on_weight(weight, keyword, values_pair, i, j);

            j--;
            i++;
        } else {
            break;
        }
    }

    next_pivot = median_of_three(weight[low], weight[(low + j) / 2], weight[j]);
    internal_quicksort(weight, keyword, values_pair, low, j, next_pivot);
    next_pivot = median_of_three(weight[i], weight[(i + high) / 2], weight[high]);
    internal_quicksort(weight, keyword, values_pair, i, high, next_pivot);
}

Status weight_based_quicksort(const FileString *filestring, KeywordData *keyword_data, unsigned short *weight) {
    Keyword *temp_keyword = NULL;
    unsigned char *temp_keyword_column = NULL;
    unsigned short *temp_values_pair = NULL;
    unsigned short *temp_weight = NULL;
    unsigned short pivot = 0;
    size_t i, j = 0;

    temp_keyword = (Keyword *) malloc(sizeof(Keyword) * keyword_data->keyword_count * keyword_data->longest_keyword_len);
    if (temp_keyword == NULL) {
        return ALLOCATION_ERROR;
    }

    temp_keyword_column = (unsigned char *) malloc(sizeof(unsigned char) * keyword_data->keyword_count * keyword_data->longest_keyword_len);
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

    memcpy(temp_keyword, keyword_data->keywords, sizeof(Keyword) * keyword_data->keyword_count);
    memcpy(temp_values_pair, keyword_data->values_pair, sizeof(unsigned short) * keyword_data->keyword_count);
    memcpy(temp_weight, weight, sizeof(unsigned short) * keyword_data->keyword_count);

    pivot = median_of_three(temp_weight[0], temp_weight[(keyword_data->keyword_count-1) / 2], temp_weight[keyword_data->keyword_count-1]);
    internal_quicksort(temp_weight, temp_keyword, temp_values_pair, 0, keyword_data->keyword_count-1, pivot);

    for (i = 0; i < keyword_data->keyword_count; i++) {
        for (j = 0; j < temp_keyword[i].len; j++) {
            temp_keyword_column[j * keyword_data->keyword_count + i] = (unsigned char) filestring->start[temp_keyword[i].start + j];
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
