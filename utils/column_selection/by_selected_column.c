#include "column_selection/by_selected_column.h"
#include "column_selection/column_selection.h"
#include "grouping/selected_column.h"
#include "char_manip.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Alphabet_Grouping_Buffer {
    unsigned char uppercase[26];
    unsigned char lowercase[26];
    unsigned char uppercase_start[26];
    unsigned char lowercase_start[26];
}Alphabet_Grouping_Buffer;


void generating_alphabet_grouping_based_on_selected_column_char(
    const Keyword_Data data,
    const unsigned char start,
    const unsigned char count,
    const unsigned char selected_column,
    Alphabet_Grouping_Buffer *alphabet
){
    unsigned char i = 0;
    unsigned char prev = 0;
    unsigned char offset = 0;
    char selected_char = 0;

    memset(alphabet, 0, sizeof(Alphabet_Grouping_Buffer));

    for (i = start; i < start + count; i++) {
        selected_char = data.keywords[i * data.longest_keyword_len + selected_column];
        if (is_lowercase_alphabet(selected_char)) {
            alphabet->lowercase[selected_char - 'a']++;
        } else {
            alphabet->uppercase[selected_char - 'A']++;
        }
    }

    for (i = 0; i < 26; i++) {
        if (alphabet->lowercase[i] > 0) {
            prev = alphabet->lowercase[i];
            alphabet->lowercase_start[i] = offset;
            offset += prev;
        }

        if (alphabet->uppercase[i] > 0) {
            prev = alphabet->uppercase[i];
            alphabet->uppercase_start[i] = offset;
            offset += prev;
        }
    }
}

void generating_grouping_buffer_based_on_alphabet_grouping(
    const Alphabet_Grouping_Buffer alphabet,
    Grouping_Buffer *grouping
) {
    unsigned char i = 0;

    for (i = 0; i < 26; i++) {
        if (alphabet.lowercase[i] > 0) {
            insert_grouping_buffer(grouping, alphabet.lowercase[i]);
        }
    }

    for (i = 0; i < 26; i++) {
        if (alphabet.uppercase[i] > 0) {
            insert_grouping_buffer(grouping, alphabet.uppercase[i]);
        }
    }
}


/* Grouping_By_Selected_Column row_grouping_based_on_selected_column(
    Column_Selection_Internal_Result *src_data,
    unsigned short grouping_start,
    unsigned short grouping_count
) {
    Grouping_By_Selected_Column result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0, 0}, {NULL, NULL, 0, 0}
    };

    Grouping_Buffer_Status new_grouping = {
        NO_ERROR, {NULL, NULL, 0, 0}
    };

    Internal_Grouping_By_Selected_Column internal_result = {
        NO_ERROR, {NULL, NULL, NULL}
    };

    Alphabet_Grouping_Buffer alphabet = {0};
    unsigned short i = 0;

    new_grouping = allocate_grouping_buffer(
        src_data->data.keyword_count
    );

    for (i = grouping_start; i < grouping_start + grouping_count; i++) {
        generating_alphabet_grouping_based_on_selected_column_char(
            src_data->data,
            src_data->grouping.start[i],
            src_data->grouping.count[i],
            src_data->selected_column,
            &alphabet
        );

        generating_grouping_buffer_based_on_alphabet_grouping(
            alphabet,
            &new_grouping.buffer
        );

        if (src_data->grouping.count[i] > 1) {
            internal_result = grouping_by_selected_column(
                src_data->data,
                src_data->selected_column,
                alphabet.uppercase_start,
                alphabet.lowercase_start,
                src_data->grouping.start[i],
                src_data->grouping.count[i]
            );

            memcpy(
                src_data->data.keywords + src_data->grouping.start[i] * src_data->data.longest_keyword_len,
                internal_result.data.keywords,
                sizeof(unsigned char) * src_data->data.longest_keyword_len * src_data->grouping.count[i]
            );

            memcpy(
                src_data->data.keywords_length + src_data->grouping.start[i],
                internal_result.data.keywords_length,
                sizeof(unsigned char) * src_data->grouping.count[i]
            );

            memcpy(
                src_data->data.values_pair + src_data->grouping.start[i],
                internal_result.data.values_pair,
                sizeof(unsigned short) * src_data->grouping.count[i]
            );

            free_partial_keyword_data(&internal_result.data);
        }

    }

    result.data = src_data->data;
    result.grouping = new_grouping.buffer;

    return result;
} */

Keyword_Data__Grouping row_grouping_based_on_selected_column(
    Keyword_Data data,
    Grouping_Buffer grouping,
    unsigned char selected_column,
    unsigned short grouping_start,
    unsigned short grouping_count
) {
    Keyword_Data__Grouping result = {
        {NULL, NULL, NULL, 0, 0, 0}, {NULL, NULL, 0, 0}, NO_ERROR
    };

    Grouping_Buffer_Status new_grouping = {
        NO_ERROR, {NULL, NULL, 0, 0}
    };

    Internal_Grouping_By_Selected_Column internal_result = {
        NO_ERROR, {NULL, NULL, NULL}
    };

    Alphabet_Grouping_Buffer alphabet = {0};
    unsigned short i = 0;

    new_grouping = allocate_grouping_buffer(
        data.keyword_count
    );

    if (new_grouping.status != NO_ERROR) {
        result.status = new_grouping.status;
        return result;
    }

    for (i = grouping_start; i < grouping_start + grouping_count; i++) {
        generating_alphabet_grouping_based_on_selected_column_char(
            data,
            grouping.start[i],
            grouping.count[i],
            selected_column,
            &alphabet
        );

        generating_grouping_buffer_based_on_alphabet_grouping(
            alphabet,
            &new_grouping.buffer
        );

        if (grouping.count[i] > 1) {
            internal_result = grouping_by_selected_column(
                data,
                selected_column,
                alphabet.uppercase_start,
                alphabet.lowercase_start,
                grouping.start[i],
                grouping.count[i]
            );

            memcpy(
                data.keywords + grouping.start[i] * data.longest_keyword_len,
                internal_result.data.keywords,
                sizeof(unsigned char) * data.longest_keyword_len * grouping.count[i]
            );

            memcpy(
                data.keywords_length + grouping.start[i],
                internal_result.data.keywords_length,
                sizeof(unsigned char) * grouping.count[i]
            );

            memcpy(
                data.values_pair + grouping.start[i],
                internal_result.data.values_pair,
                sizeof(unsigned short) * grouping.count[i]
            );

            free_partial_keyword_data(&internal_result.data);
        }

    }

    result.data = data;
    result.grouping = new_grouping.buffer;

    return result;
}
