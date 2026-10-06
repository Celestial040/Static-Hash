#include "parser.h"
#include "file_loader.h"
#include "lexer.h"
#include "status.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "char_manip.h"
#include "sort/descending/most_collide.h"
#include "sort/descending/significant_column.h"
#include "vector/landing_spot.h"

typedef unsigned char MappingMode;
enum {
    KEY_ENUMERATION,
    KEY_VALUE_PAIR
};


Status header_parse(const FileString *filestring, size_t *line_count, MappingMode *mapping_mode) {
    TokenPointer tokenpointer;
    const char *expected_headers[3] = {"struct", "key", "value"};
    unsigned char headers_loop = 0;

    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_OPEN_CURLY_BRACKET) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_IDENTIFIER) {
        return MISMATCH_EXPECTATION;
    }

    for (; headers_loop < 2; headers_loop++) {
        if (strncmp(filestring->start + tokenpointer.start, expected_headers[headers_loop], tokenpointer.len) == 0) {
            break;
        }
    }

    if (headers_loop == 2) {
        return MISMATCH_EXPECTATION;
    }


    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_CLOSE_CURLY_BRACKET) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring);


    if (tokenpointer.token != TOKEN_COMMA && tokenpointer.token != TOKEN_ENDLINE) {
        return MISMATCH_EXPECTATION;
    }

    if (headers_loop == 1 && tokenpointer.token == TOKEN_COMMA) {
        *mapping_mode = KEY_VALUE_PAIR;
    } else {
        *mapping_mode = KEY_ENUMERATION;
        return NO_ERROR;
    }

    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_OPEN_CURLY_BRACKET) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_IDENTIFIER) {
        return MISMATCH_EXPECTATION;
    }

    if (strncmp(filestring->start + tokenpointer.start, expected_headers[2], tokenpointer.len) != 0) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_CLOSE_CURLY_BRACKET) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_ENDLINE) {
        return MISMATCH_EXPECTATION;
    }

    *line_count += 1;

    return NO_ERROR;
}

Status body_parse_key_value(const FileString *filestring, size_t *line_count, KeywordData *result_output) {
    TokenPointer tokenpointer;
    Keyword *keywords = NULL;
    unsigned char *keyword_column = NULL;
    unsigned short *values_pair = NULL;

    size_t body_start_index = get_lexer_tail();
    size_t i , j = 0;

    Status status = NO_ERROR;

    size_t keyword_count = 0;
    unsigned char longest_keyword_len = 0;


    while (1) {
        tokenpointer = lexer_scan(filestring);
        if (keyword_count > 0 && tokenpointer.token == TOKEN_EOF) {
            break;
        }

        *line_count += 1;

        if (tokenpointer.token != TOKEN_IDENTIFIER) {
            return MISMATCH_EXPECTATION;
        }

        if (tokenpointer.len > 255) {
            return STRING_TOO_LONG;
        }

        if (tokenpointer.len > longest_keyword_len) {
            longest_keyword_len = (unsigned char) tokenpointer.len;
        }

        tokenpointer = lexer_scan(filestring);

        if (tokenpointer.token != TOKEN_COMMA) {
            return MISMATCH_EXPECTATION;
        }

        tokenpointer = lexer_scan(filestring);

        if (tokenpointer.token != TOKEN_NUMERIC_LITERAL) {
            return MISMATCH_EXPECTATION;
        }

        tokenpointer = lexer_scan(filestring);

        if (tokenpointer.token != TOKEN_ENDLINE && tokenpointer.token != TOKEN_EOF) {
            return MISMATCH_EXPECTATION;
        }

        keyword_count++;
    }

    keywords = (Keyword *) malloc(sizeof(Keyword) * keyword_count);
    if (keywords == NULL) {
        return ALLOCATION_ERROR;
    }

    keyword_column = (unsigned char *) calloc(keyword_count * longest_keyword_len , sizeof(char));
    if (keyword_column == NULL) {
        return ALLOCATION_ERROR;
    }

    values_pair = (unsigned short *) malloc(sizeof(unsigned short) * keyword_count);
    if (values_pair == NULL ) {
        return ALLOCATION_ERROR;
    }

    set_lexer_index(body_start_index);

    for (i = 0; i < keyword_count; i++) {

        /* keyword */
        tokenpointer = lexer_scan(filestring);

        keywords[i].start = tokenpointer.start;
        keywords[i].len = tokenpointer.len;

        for (j = 0; j < tokenpointer.len; j++) {
            keyword_column[j * keyword_count + i] = (unsigned char) filestring->start[tokenpointer.start + j];
        }

        /* comma */
        tokenpointer = lexer_scan(filestring);

        /* numeric value*/
        tokenpointer = lexer_scan(filestring);
        status = string_to_unsigned_short(&filestring->start[tokenpointer.start], tokenpointer.len, &values_pair[i]);
        if (status != NO_ERROR) {
            return status;
        }

        /* endline */
        tokenpointer = lexer_scan(filestring);

    }

    result_output->keywords = keywords;
    result_output->keyword_column = keyword_column;
    result_output->values_pair = values_pair;
    result_output->keyword_count = keyword_count;
    result_output->longest_keyword_len = longest_keyword_len;

    return NO_ERROR;
}

Status sort_keywords(const FileString *filestring, KeywordData *data) {
    Status status = NO_ERROR;
    unsigned short lowercase[26] = {0};
    unsigned short uppercase[26] = {0};
    unsigned short *keyword_score = NULL;
    size_t i,j = 0;


    keyword_score = (unsigned short *) calloc(data->keyword_count,sizeof(unsigned short));
    if (keyword_score == NULL) {
        return ALLOCATION_ERROR;
    }

    for (i = 0; i < data->longest_keyword_len; i++) {
        for (j = i*data->keyword_count; j < (i+1)*data->keyword_count; j++) {

            if (data->keyword_column[j] == 0) {
                continue;
            }

            if (data->keyword_column[j] >= 'a') {
                lowercase[data->keyword_column[j] - 'a'] += 1;
            } else {
                uppercase[data->keyword_column[j] - 'A'] += 1;
            }
        }

        for (j = 0; j < data->keyword_count; j++) {
            if (data->keyword_column[j] == 0) {
                continue;
            }

            if (data->keyword_column[j] >= 'a') {
                keyword_score[j] += lowercase[data->keyword_column[j] - 'a'];
            } else {
                keyword_score[j] += uppercase[data->keyword_column[j] - 'A'];
            }
        }

        for (j = 0; j < 26; j++) {
            lowercase[j] = 0;
            uppercase[j] = 0;
        }
    }

    status = most_collide_keyword_sort(filestring, data, keyword_score);
    if (status != NO_ERROR) {
        return status;
    }

    return NO_ERROR;
}

Status finding_most_significant_column(KeywordData *data, unsigned char **significant_column_order) {
    Status status = NO_ERROR;
    unsigned char lowercase[26] = {0};
    unsigned char uppercase[26] = {0};
    unsigned char *column_uniqueness_score = NULL;
    unsigned char *column_index = NULL;
    size_t i, j = 0;

    column_uniqueness_score = (unsigned char *) calloc(data->longest_keyword_len,sizeof(unsigned char));
    if (column_uniqueness_score == NULL) {
        return ALLOCATION_ERROR;
    }

    column_index = (unsigned char *) malloc(sizeof(unsigned char) * data->longest_keyword_len);
    if (column_index == NULL) {
        return ALLOCATION_ERROR;
    }

    for (i = 0; i < data->longest_keyword_len; i++) {
        column_index[i] = (unsigned char) i;
    }

    for (i = 0; i < data->longest_keyword_len; i++) {
        for (j = i*data->keyword_count; j < (i+1)*data->keyword_count; j++) {

            if (data->keyword_column[j] == 0) {
                continue;
            }

            if (is_lowercase_alphabet((char)data->keyword_column[j])) {
                if (lowercase[data->keyword_column[j] - 'a'] == 0) {
                    lowercase[data->keyword_column[j] - 'a'] = 1;
                }

            } else {
                if (uppercase[data->keyword_column[j] - 'A'] == 0) {
                    uppercase[data->keyword_column[j] - 'A'] = 1;
                }
            }
        }

        for (j = 0; j < 26; j++) {
            column_uniqueness_score[i] += lowercase[j];
            column_uniqueness_score[i] += uppercase[j];

            lowercase[j] = 0;
            uppercase[j] = 0;
        }
    }

    status = significant_column_sort(&column_uniqueness_score, &column_index, data->longest_keyword_len);
    if (status != NO_ERROR) {
        return status;
    }

    free(column_uniqueness_score);

    *significant_column_order = column_index;

    return NO_ERROR;
}

Status calculating_static_hash(const KeywordData *data, const unsigned char *significant_column_order) {
    Status allocation_status = NO_ERROR;
    unsigned short *scoreboard = NULL;
    unsigned short *score_lookup = NULL;
    unsigned short *scoreboard_total = NULL;
    size_t target_index = 0;
    size_t highest_slot_index = 0;
    unsigned char selected_char = 0;
    LandingSpotVector landing_spot;
    size_t i,j,k = 0;

    scoreboard = (unsigned short *) calloc(data->keyword_count * data->longest_keyword_len, sizeof(unsigned short));
    if (scoreboard == NULL) {
        return ALLOCATION_ERROR;
    }

    score_lookup = (unsigned short *) calloc(26 * 2 * data->longest_keyword_len, sizeof(unsigned short));
    if (score_lookup == NULL) {
        return ALLOCATION_ERROR;
    }

    scoreboard_total = (unsigned short *) calloc(data->keyword_count, sizeof(unsigned short));
    if (scoreboard_total == NULL) {
        return ALLOCATION_ERROR;
    }

    allocation_status = allocate_landing_spot_vector(&landing_spot, 1024*5);
    if (allocation_status != NO_ERROR) {
        return allocation_status;
    }

    for (i = 0; i < 1024*5; i++) {
        landing_spot.array[i] = 0;
    }

    for (i = 0; i < data->longest_keyword_len; i++) {
        for (j = 0; j < data->keyword_count; j++) {
            target_index = significant_column_order[0] * data->keyword_count + j;
            selected_char = data->keyword_column[target_index];

            if (is_lowercase_alphabet((char)selected_char)) {
                scoreboard[j * data->keyword_count + significant_column_order[0]] += score_lookup[data->keyword_column[target_index] - 'a'];
            } else {
                scoreboard[j * data->keyword_count + significant_column_order[0]] += score_lookup[data->keyword_column[target_index] - 'A'];
            }
        }

        for (j = 0; j < data->keyword_count; j++) {
            for (k = 0; k < data->longest_keyword_len; k++) {
                scoreboard_total[j] += scoreboard[j * data->keyword_count + k];
            }

            scoreboard_total[j] += data->keywords[j].len;
            if (landing_spot.capacity < scoreboard_total[j]) {
                reallocate_landing_spot_vector(&landing_spot, landing_spot.capacity*10);
            }

            if (landing_spot.array[scoreboard_total[j]] == 0) {
                landing_spot.array[scoreboard_total[j]] = 1;
            } else {

            }
        }

        break;
    }

    return NO_ERROR;
}

Status parser_start(const FileString *filestring) {
    size_t line_count = 0;
    MappingMode mapping_mode = KEY_ENUMERATION;
    Status status = NO_ERROR;
    KeywordData keyword_data = {NULL,NULL,NULL,0,0};
    unsigned char *significant_column_order = NULL;

    status = header_parse(filestring, &line_count, &mapping_mode);
    if (status != NO_ERROR) {return status;}


    switch (mapping_mode) {
        case KEY_VALUE_PAIR:
            status = body_parse_key_value(filestring, &line_count, &keyword_data);
            if (status != NO_ERROR) {return status;}
            break;

        default:
            break;
    }

    status = sort_keywords(filestring,&keyword_data);
    if (status != NO_ERROR) {return status;}
    status = finding_most_significant_column(&keyword_data,&significant_column_order);
    if (status != NO_ERROR) {return status;}
    status = calculating_static_hash(&keyword_data,significant_column_order);
    if (status != NO_ERROR) {return status;}

    return status;
}
