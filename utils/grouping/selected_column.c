#include "status.h"
#include "char_manip.h"
#include "grouping/selected_column.h"

#include <stdio.h>
#include <string.h>

Internal_Grouping_By_Selected_Column grouping_by_selected_column(
    const Keyword_Data data,
    const unsigned char selected_column,
    const unsigned char *uppercase_start,
    const unsigned char *lowercase_start,
    const unsigned char start,
    const unsigned char length
) {

    Internal_Grouping_By_Selected_Column result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0, 0}
    };

    Keyword_Data_Status new_data = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0, 0}
    };

    unsigned char uppercase_start_copy[26] = {0};
    unsigned char lowercase_start_copy[26] = {0};

    unsigned char i = 0;
    unsigned char target_index = 0;
    char selected_char = 0;

    memcpy(uppercase_start_copy, uppercase_start, 26);
    memcpy(lowercase_start_copy, lowercase_start, 26);


    new_data = copy_keyword_data(data);

    for (i = start; i < length; i++) {
        selected_char = data.keywords[i * data.longest_keyword_len + selected_column];
        if (is_lowercase_alphabet(selected_char) == 1) {
            target_index = lowercase_start_copy[selected_char - 'a'];
            lowercase_start_copy[selected_char - 'a']++;
        } else {
            target_index = uppercase_start_copy[selected_char - 'A'];
            uppercase_start_copy[selected_char - 'A']++;
        }

        memcpy(
            new_data.data.keywords + target_index * data.longest_keyword_len,
            data.keywords + i * data.longest_keyword_len,
            data.keywords_length[i]
        );

        new_data.data.keywords_length[target_index] = data.keywords_length[i];
        new_data.data.values_pair[target_index] = data.values_pair[i];
    }

    result.data = new_data.data;
    return result;

}
