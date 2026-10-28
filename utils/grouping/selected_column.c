#include "parser.h"
#include "status.h"
#include "char_manip.h"
#include "grouping/selected_column.h"

#include <stdio.h>
#include <string.h>

Internal_Grouping_BySelectedColumn grouping_by_selected_column( const KeywordData data,
                                                                const unsigned char selected_column,
                                                                const unsigned char *uppercase_start,
                                                                const unsigned char *lowercase_start,
                                                                const unsigned char start,
                                                                const unsigned char length)
{

    Internal_Grouping_BySelectedColumn result = {NO_ERROR, NULL, NULL, NULL};
    unsigned char i = 0;

    char *new_keywords = NULL;
    unsigned char *new_keywords_length = NULL;
    unsigned short *new_values_pair = NULL;

    unsigned char uppercase_start_copy[26] = {0};
    unsigned char lowercase_start_copy[26] = {0};

    unsigned char target_index = 0;
    char selected_char = 0;

    new_keywords = (char *) calloc(length * data.longest_keyword_len,sizeof(char));
    if (new_keywords == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    new_keywords_length = (unsigned char *) malloc(sizeof(unsigned char) * length);
    if (new_keywords_length == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    new_values_pair = (unsigned short *) malloc(sizeof(unsigned char) * length);
    if (new_values_pair == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    memcpy(uppercase_start_copy, uppercase_start, 26);
    memcpy(lowercase_start_copy, lowercase_start, 26);

    for (i = start; i < length; i++) {
        selected_char = data.keywords[i * data.longest_keyword_len + selected_column];
        if (is_lowercase_alphabet(selected_char) == 1) {
            target_index = lowercase_start_copy[selected_char - 'a'];
            lowercase_start_copy[selected_char - 'a']++;
        } else {
            target_index = uppercase_start_copy[selected_char - 'A'];
            uppercase_start_copy[selected_char - 'A']++;
        }

        memcpy(new_keywords + target_index * data.longest_keyword_len, data.keywords + i * data.longest_keyword_len , data.keywords_length[i]);
        new_keywords_length[target_index] = data.keywords_length[i];
        new_values_pair[target_index] = data.values_pair[i];
    }

    result.keywords = new_keywords;
    result.keywords_length = new_keywords_length;
    result.values_pair = new_values_pair;
    return result;

}


/* Status grouping_by_selected_column(KeywordData *data, unsigned char selected_column, unsigned char *uppercase_start, unsigned char *lowercase_start,
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
 */
