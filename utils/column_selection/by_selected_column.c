#include "column_selection/by_selected_column.h"
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

void filling_alphabet_grouping_by_selected_column(
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

void filling_grouping_buffer_based_on_alphabet_grouping_buffer(
    const Alphabet_Grouping_Buffer alphabet,
    Grouping_Buffer grouping_buffer,
    unsigned char *insertion_count
) {
    unsigned char i = 0;

    for (i = 0; i < 26; i++) {
        if (alphabet.lowercase[i] > 0) {
            grouping_buffer.grouping_count[*insertion_count] = alphabet.lowercase[i];
            if (*insertion_count > 0) {
                grouping_buffer.grouping_start_index[*insertion_count] += grouping_buffer.grouping_start_index[*insertion_count - 1];
                grouping_buffer.grouping_start_index[*insertion_count] += + grouping_buffer.grouping_count[*insertion_count - 1];
            }
            printf("%d \n", *insertion_count);
            (*insertion_count)++;
        }
    }

    for (i = 0; i < 26; i++) {
        if (alphabet.uppercase[i] > 0) {
            grouping_buffer.grouping_count[*insertion_count] = alphabet.uppercase[i];
            if (*insertion_count > 0) {
                grouping_buffer.grouping_start_index[*insertion_count] += grouping_buffer.grouping_start_index[*insertion_count - 1];
                grouping_buffer.grouping_start_index[*insertion_count] += grouping_buffer.grouping_count[*insertion_count - 1];
            }
            (*insertion_count)++;
        }
    }
}

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

    Alphabet_Grouping_Buffer alphabet = {0};

    unsigned char i = 0;
    unsigned char insertion_count = 0;

    new_grouping_buffer = allocate_grouping_buffer(
        data.keyword_count
    );


    for (i = data.shortest_keyword_len; i < data.longest_keyword_len + 1; i++) {
        filling_alphabet_grouping_by_selected_column(
            data,
            grouping_buffer.grouping_start_index[i],
            grouping_buffer.grouping_count[i],
            selected_column,
            &alphabet
        );

        filling_grouping_buffer_based_on_alphabet_grouping_buffer(
            alphabet,
            new_grouping_buffer.buffer,
            &insertion_count
        );
    }

    for (i = 0; i < data.keyword_count; i++) {
        printf("%d , %d \n", new_grouping_buffer.buffer.grouping_start_index[i], new_grouping_buffer.buffer.grouping_count[i]);
    }

    return result;
}


Grouping_By_Selected_Column row_grouping_by_selected_column(
    const Keyword_Data data,
    const Grouping_Buffer grouping_buffer,
    const unsigned char selected_column
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
