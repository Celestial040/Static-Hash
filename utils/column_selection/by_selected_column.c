#include "column_selection/by_selected_column.h"
#include "column_selection/column_pick.h"
#include "char_manip.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Grouping_Buffer_Status filling_grouping_buffer_by_selected_column(
    const Keyword_Data data,
    const Grouping_Buffer grouping_buffer,
    const unsigned char selected_column
) {
    Grouping_Buffer_Status result = {
        NO_ERROR, {NULL, NULL, 0}
    };

    Grouping_Buffer_Status new_grouping_buffer = {
        NO_ERROR, {NULL, NULL, 0}
    };

    unsigned char i,j = 0;

    unsigned char prev = 0;
    unsigned char offset = 0;

    unsigned char uppercase[26] = {0};
    unsigned char lowercase[26] = {0};

    unsigned char uppercase_start[26] = {0};
    unsigned char lowercase_start[26] = {0};

    char selected_char = 0;

    unsigned char insertion_count = 0;

    new_grouping_buffer = allocate_grouping_buffer(
        data.keyword_count
    );

    for (i = data.shortest_keyword_len; i < data.longest_keyword_len + 1; i++) {
        for (j = grouping_buffer.grouping_start_index[i]; j < grouping_buffer.grouping_start_index[i] + grouping_buffer.grouping_count[i]; j++) {
            selected_char = data.keywords[j * data.longest_keyword_len + selected_column];
            if (is_lowercase_alphabet(selected_char)) {
                lowercase[selected_char - 'a']++;
            } else {
                uppercase[selected_char - 'A']++;
            }
        }

        for (j = 0; j < 26; j++) {
            if (lowercase[j] > 0) {
                prev = lowercase[j];
                lowercase_start[j] = offset;
                offset += prev;
            }

            if (uppercase[j] > 0) {
                prev = uppercase[j];
                uppercase_start[j] = offset;
                offset += prev;
            }
        }

        for (j = 0; j < 26; j++) {
            if (lowercase[j] > 0) {
                printf("%c = %d ", 'a' + j, lowercase[j]);
                printf("%c = %d\n", 'a' + j, lowercase_start[j]);
                new_grouping_buffer.buffer.grouping_count[insertion_count] = lowercase[j];
                new_grouping_buffer.buffer.grouping_start_index[insertion_count] = lowercase_start[j];
                if (insertion_count > 0) {
                    new_grouping_buffer.buffer.grouping_start_index[insertion_count] += new_grouping_buffer.buffer.grouping_start_index[insertion_count - 1];
                }
                insertion_count++;
            }
        }

        for (j = 0; j < 26; j++) {
            if (uppercase[j] > 0) {
                printf("%c = %d ",'A' + j, uppercase[j]);
                printf("%c = %d\n",'A' + j, uppercase_start[j]);
                new_grouping_buffer.buffer.grouping_count[insertion_count] = uppercase[j];
                new_grouping_buffer.buffer.grouping_start_index[insertion_count] = uppercase_start[j];
                if (insertion_count > 0) {
                    new_grouping_buffer.buffer.grouping_start_index[insertion_count] += new_grouping_buffer.buffer.grouping_start_index[insertion_count - 1];
                }
                insertion_count++;
            }
        }

        memset(lowercase, 0, 26);
        memset(lowercase_start, 0, 26);
        memset(uppercase, 0, 26);
        memset(uppercase_start, 0, 26);
    }



    return result;
}


Grouping_By_Selected_Column row_grouping_by_selected_column(
    const Keyword_Data data,
    const Grouping_Buffer grouping_buffer
) {
    Grouping_By_Selected_Column result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0, 0}, {NULL, NULL, 0}
    };

    Keyword_Data_Status new_data = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0, 0}
    };

    Grouping_Buffer_Status new_grouping_buffer = {
        NO_ERROR, {NULL, NULL, 0}
    };

    unsigned char selected_column = column_pick_based_on_length(
        data,
        grouping_buffer.grouping_count,
        grouping_buffer.grouping_start_index
    );

    new_data = copy_keyword_data(data);
    if (new_data.status != NO_ERROR) {
        result.status = new_data.status;
        return result;
    }

    new_grouping_buffer = filling_grouping_buffer_by_selected_column(
        new_data.data,
        grouping_buffer,
        selected_column
    );

    result.data = new_data.data;
    result.grouping_buffer = new_grouping_buffer.buffer;
    return result;
}
