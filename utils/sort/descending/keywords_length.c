#include <stddef.h>
#include "status.h"
#include "sort/median.h"
#include "sort/descending/keywords_length.h"


void swap_sort_base_on_keywords_length(KeywordData *keyword_data, size_t index_1, size_t index_2) {
    unsigned char i = 0;

    char *keywords = keyword_data->keywords;
    unsigned char *keywords_length = keyword_data->keywords_length;
    unsigned short *values_pair = keyword_data->values_pair;

    char temp_char = 0;
    unsigned char temp_keywords_length = 0;
    unsigned short temp_values_pair = 0;

    temp_keywords_length = keywords_length[index_1];
    keywords_length[index_1] = keywords_length[index_2];
    keywords_length[index_2] = temp_keywords_length;

    temp_values_pair = values_pair[index_1];
    values_pair[index_1] = values_pair[index_2];
    values_pair[index_2] = temp_values_pair;

    for (i = 0; i < keyword_data->longest_keyword_len; i++) {
        temp_char = keywords[index_1 * keyword_data->longest_keyword_len + i];
        keywords[index_1 * keyword_data->longest_keyword_len + i] = keywords[index_2 * keyword_data->longest_keyword_len + i];
        keywords[index_2 * keyword_data->longest_keyword_len + i] = temp_char;
    }

/*     temp_keywords = keywords[index_1];
    keywords[index_1] = keywords[index_2];
    keywords[index_2] = temp_keywords;
 */


}

void keywords_length_internal_quicksort(KeywordData *keyword_data, unsigned char low, unsigned char high, unsigned short pivot) {
    unsigned char *keywords_length = keyword_data->keywords_length;

    unsigned char i = low;
    unsigned char j = high;
    unsigned short next_pivot = 0;

    if (low == high) {
        return;
    }

    if (high - low == 1) {
        if (keywords_length[low] < keywords_length[high]) {
            swap_sort_base_on_keywords_length(keyword_data, low, high);
        }
        return;
    }

    while (1) {
        while (keywords_length[i] > pivot) {
            i++;
        }

        while (keywords_length[j] < pivot) {
            j--;
        }

        if (i<j) {

            swap_sort_base_on_keywords_length(keyword_data, i, j);

            j--;
            i++;
        } else {
            break;
        }
    }

    next_pivot = median_of_three(keywords_length[low], keywords_length[(low + j) / 2], keywords_length[j]);
    keywords_length_internal_quicksort(keyword_data, low, j, next_pivot);

    next_pivot = median_of_three(keywords_length[i], keywords_length[(i + high) / 2], keywords_length[high]);
    keywords_length_internal_quicksort(keyword_data, i, high, next_pivot);
}

Status keywords_length_sort(KeywordData *keyword_data) {
    unsigned short pivot = 0;
    unsigned char *keywords_length = keyword_data->keywords_length;

    unsigned char low = 0;
    unsigned char mid = (keyword_data->keyword_count-1) / 2;
    unsigned char high = keyword_data->keyword_count-1;

    pivot = median_of_three(keywords_length[low], keywords_length[mid], keywords_length[high]);
    keywords_length_internal_quicksort(keyword_data, low, high, pivot);

    return NO_ERROR;
}
