#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "status.h"
#include "sort/median.h"



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

Status significant_column_sort(unsigned char **column_uniqueness, unsigned char **column_index , size_t array_len) {
    unsigned char *temp_column_uniqueness = NULL;
    unsigned char *temp_column_index = NULL;
    unsigned short pivot = 0;

    temp_column_uniqueness = (unsigned char *) malloc(sizeof(unsigned char) * array_len);
    if (temp_column_uniqueness == NULL) {
        return ALLOCATION_ERROR;
    }

    temp_column_index = (unsigned char *) malloc(sizeof(unsigned char) * array_len);
    if (temp_column_index == NULL) {
        return ALLOCATION_ERROR;
    }

    memcpy(temp_column_uniqueness, *column_uniqueness, sizeof(unsigned char) * array_len);
    memcpy(temp_column_index, *column_index, sizeof(unsigned char) * array_len);

    pivot = median_of_three(temp_column_uniqueness[0], temp_column_uniqueness[(array_len-1) / 2], temp_column_uniqueness[array_len-1]);
    column_uniqueness_internal_quicksort( temp_column_uniqueness, temp_column_index, 0, array_len-1, pivot);

    free(*column_index);
    *column_index = temp_column_index;

    free(*column_uniqueness);
    *column_uniqueness = temp_column_uniqueness;

    return NO_ERROR;
}
