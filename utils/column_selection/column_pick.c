#include "char_manip.h"
#include <stddef.h>
#include "column_selection/column_pick.h"

unsigned short column_score_calculation_based_on_length(
    const Keyword_Data data,
    const unsigned char group_start,
    const unsigned char group_count,
    const unsigned char column_index
){
    unsigned short column_separation_score = 0;

    unsigned char uppercase[26] = {0};
    unsigned char lowercase[26] = {0};

    char selected_char = 0;
    unsigned char i = 0;

    column_separation_score = group_count * (group_count - 1) / 2;

    for (i = group_start; i < group_start + group_count ; i++) {
        selected_char = data.keywords[i * data.longest_keyword_len + column_index];
        if (is_lowercase_alphabet(selected_char) == 1) {
            lowercase[selected_char - 'a']++;
        } else {
            uppercase[selected_char - 'A']++;
        }
    }

    for (i = 0; i < 26; i++) {
        if (lowercase[i] > 1) {
            column_separation_score -= lowercase[i] * (lowercase[i] - 1) / 2;
        }

        if (uppercase[i] > 1) {
            column_separation_score -= uppercase[i] * (uppercase[i] - 1) / 2;
        }
    }

    return column_separation_score;
}

unsigned char column_pick_based_on_length(
    const Keyword_Data data,
    const unsigned char *group_count,
    const unsigned char *group_start
) {

    unsigned short best_column_score = 0;
    unsigned short current_column_score = 0;
    unsigned char best_column = 0;


    unsigned char i = 0;

    for (i = data.shortest_keyword_len; i <= data.longest_keyword_len; i++) {
        current_column_score = column_score_calculation_based_on_length(
            data, group_start[i], group_count[i], i
        );

        if (current_column_score > best_column_score) {
            best_column_score = current_column_score;
            best_column = i;
        }
    }

    return best_column;
}
