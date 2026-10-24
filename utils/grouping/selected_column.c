#include "parser.h"
#include "status.h"
#include "char_manip.h"

#include <stdio.h>
#include <string.h>


Status grouping_by_selected_column(KeywordData *data, unsigned char selected_column, unsigned char *uppercase_start, unsigned char *lowercase_start,
                                    unsigned char start, unsigned char length) {
    unsigned char i = 0;

    char *keywords = NULL;
    unsigned char *keywords_length = NULL;
    unsigned short *values_pair = NULL;
    unsigned char target_index = 0;
    char selected_char = 0;

    keywords = (char *) calloc(length * data->longest_keyword_len,sizeof(char));
    if (keywords == NULL) {
        return ALLOCATION_ERROR;
    }

    keywords_length = (unsigned char *) malloc(sizeof(unsigned char) * length);
    if (keywords_length == NULL) {
        return ALLOCATION_ERROR;
    }

    values_pair = (unsigned short *) malloc(sizeof(unsigned char) * length);
    if (values_pair == NULL) {
        return ALLOCATION_ERROR;
    }


    for (i = start; i < length; i++) {
        selected_char = data->keywords[i * data->longest_keyword_len + selected_column];
        if (is_lowercase_alphabet(selected_char) == 1) {
            target_index = lowercase_start[selected_char - 'a'];
            lowercase_start[selected_char - 'a']++;
        } else {
            target_index = uppercase_start[selected_char - 'A'];
            uppercase_start[selected_char - 'A']++;
        }

        memcpy(keywords + target_index * data->longest_keyword_len, data->keywords + i * data->longest_keyword_len , data->keywords_length[i]);
        keywords_length[target_index] = data->keywords_length[i];
        values_pair[target_index] = data->values_pair[i];
    }


    memcpy(data->keywords + start * data->longest_keyword_len, keywords, length * data->longest_keyword_len);
    memcpy(data->keywords_length + start, keywords_length, length);
    memcpy(data->values_pair + start, values_pair, length);


    printf("%d \n", start);
    return NO_ERROR;
}
