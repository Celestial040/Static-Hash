#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "status.h"
#include "sort/median.h"
#include "sort/descending/most_unique_column.h"


void swap_sort_base_on_column_uniqueness(unsigned char *column_uniqueness, unsigned char *column_index, size_t index_1, size_t index_2) {
    unsigned char temp_column_index = 0;
    unsigned char temp_column_uniqueness = 0;

    temp_column_index = column_index[index_1];
    column_index[index_1] = column_index[index_2];
    column_index[index_2] = temp_column_index;

    temp_column_uniqueness = column_uniqueness[index_1];
    column_uniqueness[index_1] = column_uniqueness[index_2];
    column_uniqueness[index_2] = temp_column_uniqueness;

}

void column_uniqueness_internal_quicksort(unsigned char *column_uniqueness, unsigned char *column_index, size_t low, size_t high, unsigned short pivot) {
    size_t i = low;
    size_t j = high;
    unsigned short next_pivot = 0;

    if (low == high) {
        return;
    }

    if (high - low == 1) {
        if (column_uniqueness[low] < column_uniqueness[high]) {
            swap_sort_base_on_column_uniqueness(column_uniqueness, column_index, low, high);
        }
        return;
    }

    while (1) {
        while (column_uniqueness[i] > pivot) {
            i++;
        }

        while (column_uniqueness[j] < pivot) {
            j--;
        }

        if (i<j) {

            swap_sort_base_on_column_uniqueness(column_uniqueness, column_index, i, j);

            j--;
            i++;
        } else {
            break;
        }
    }

    next_pivot = median_of_three(column_uniqueness[low], column_uniqueness[(low + j) / 2], column_uniqueness[j]);
    column_uniqueness_internal_quicksort(column_uniqueness, column_index, low, j, next_pivot);
    next_pivot = median_of_three(column_uniqueness[i], column_uniqueness[(i + high) / 2], column_uniqueness[high]);
    column_uniqueness_internal_quicksort(column_uniqueness, column_index, i, high, next_pivot);
}

Status most_unique_column_sort(KeywordData *keyword_data, unsigned char *column_uniqueness, unsigned char *column_index) {
    size_t i, j = 0;
    unsigned short pivot = 0;
    char *temp_keyword_column = NULL;

    temp_keyword_column = (char *) malloc(sizeof(char) * keyword_data->longest_keyword_len * keyword_data->keyword_count);
    if (temp_keyword_column == NULL) {
        return ALLOCATION_ERROR;
    }

    pivot = median_of_three(column_uniqueness[0], column_uniqueness[(keyword_data->keyword_count-1) / 2], column_uniqueness[keyword_data->keyword_count-1]);
    column_uniqueness_internal_quicksort(column_uniqueness, column_index, 0, keyword_data->keyword_count-1, pivot);

    for (i = 0; i < keyword_data->longest_keyword_len; i++) {
        for (j = 0; j < keyword_data->keyword_count; j++) {
            temp_keyword_column[i * keyword_data->longest_keyword_len + j] = keyword_data->keyword_column[column_index[i] * keyword_data->longest_keyword_len + j] ;
        }
    }

    free(keyword_data->keyword_column);
    keyword_data->keyword_column = temp_keyword_column;

    return NO_ERROR;
}
