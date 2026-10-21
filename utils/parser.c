#include "parser.h"
#include "file_loader.h"
#include "lexer.h"
#include "status.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "char_manip.h"
#include "grouping/keywords_length.h"
#include "sort/descending/keywords_length.h"
/* #include "vector/landing_spot.h"  */

typedef unsigned char MappingMode;
enum {
    KEY_ENUMERATION,
    KEY_VALUE_PAIR
};

void print_keyword_data(KeywordData *data) {
    size_t i, j = 0;

    for (i = 0; i < data->keyword_count; i++) {

        for (j = 0; j < data->longest_keyword_len; j++) {
            if (data->keywords[i * data->longest_keyword_len + j] == 0) {
                break;
            }
            printf("%c ",data->keywords[i * data->longest_keyword_len + j]);
        }
        printf("\n");
    }

    printf("\n");

    for (i = 0; i < data->keyword_count; i++) {
        printf("%d \n", data->keywords_length[i]);
    }

    printf("\n");


}

Status header_parse(const FileString *filestring, size_t *line_count, MappingMode *mapping_mode) {
    TokenPointer tokenpointer;

    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_OPEN_CURLY_BRACKET) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_IDENTIFIER) {
        return MISMATCH_EXPECTATION;
    }

    if (strncmp(filestring->start + tokenpointer.start, "enum", tokenpointer.len) == 0) {
        *mapping_mode = KEY_ENUMERATION;
    } else if (strncmp(filestring->start + tokenpointer.start, "key", tokenpointer.len) == 0) {
        *mapping_mode = KEY_VALUE_PAIR;
    } else {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_CLOSE_CURLY_BRACKET) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring);

    if (mapping_mode == KEY_ENUMERATION) {
        if (tokenpointer.token != TOKEN_ENDLINE) {
            return MISMATCH_EXPECTATION;
        }

        *line_count += 1;
        return NO_ERROR;
    }

    if (tokenpointer.token != TOKEN_COMMA) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_OPEN_CURLY_BRACKET) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring);

    if (tokenpointer.token != TOKEN_IDENTIFIER) {
        return MISMATCH_EXPECTATION;
    }

    if (strncmp(filestring->start + tokenpointer.start, "value", tokenpointer.len) != 0) {
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
    Status converting_string_status = NO_ERROR;

    TokenPointer tokenpointer;

    char *keywords = NULL;
    unsigned char *keywords_length = NULL;
    unsigned short *values_pair = NULL;

    size_t body_start_index = get_lexer_tail();
    size_t i , j = 0;

    unsigned char keyword_count = 0;
    unsigned char longest_keyword_len = 0;
    unsigned char shortest_keyword_len = 255;

    while (1) {
        tokenpointer = lexer_scan(filestring);

        if (keyword_count > 0 && tokenpointer.token == TOKEN_EOF) {
            *line_count += 1;
            break;
        }

        if (tokenpointer.token != TOKEN_IDENTIFIER) {
            return MISMATCH_EXPECTATION;
        }

        if (keyword_count == 255) {
            return MAX_LIMIT_EXCEEDED;
        }

        if (tokenpointer.len > 255) {
            return STRING_TOO_LONG;
        }

        if (tokenpointer.len > longest_keyword_len) {
            longest_keyword_len = (unsigned char) tokenpointer.len;
        }

        if (tokenpointer.len < shortest_keyword_len) {
            shortest_keyword_len = (unsigned char) tokenpointer.len;
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

        *line_count += 1;
        keyword_count++;
    }

    keywords = (char *) calloc(keyword_count * longest_keyword_len , sizeof(char));
    if (keywords == NULL) {
        return ALLOCATION_ERROR;
    }

    keywords_length = (unsigned char *) calloc(keyword_count , sizeof(char));
    if (keywords_length == NULL) {
        return ALLOCATION_ERROR;
    }

    values_pair = (unsigned short *) malloc(sizeof(unsigned short) * keyword_count);
    if (values_pair == NULL ) {
        return ALLOCATION_ERROR;
    }


    set_lexer_index(body_start_index);

    for (i = 0; i < keyword_count; i++) {
        tokenpointer = lexer_scan(filestring);
        keywords_length[i] = (unsigned char) tokenpointer.len;

        for (j = 0; j < tokenpointer.len; j++) {
            keywords[i * longest_keyword_len + j] =  filestring->start[tokenpointer.start + j];
        }

        tokenpointer = lexer_scan(filestring);
        tokenpointer = lexer_scan(filestring);

        converting_string_status = string_to_unsigned_short(&filestring->start[tokenpointer.start], tokenpointer.len, &values_pair[i]);
        if (converting_string_status != NO_ERROR) {
            return converting_string_status;
        }

        tokenpointer = lexer_scan(filestring);
    }

    result_output->keywords = keywords;
    result_output->keywords_length = keywords_length;
    result_output->values_pair = values_pair;
    result_output->keyword_count = keyword_count;
    result_output->longest_keyword_len = longest_keyword_len;
    result_output->shortest_keyword_len = shortest_keyword_len;

    return NO_ERROR;
}

Status column_grouping_by_length(KeywordData *data) {
    Status status = NO_ERROR;

    unsigned char i = 0;
    unsigned char *grouping_start = NULL;
    unsigned char prev = 0;
    unsigned char offset = 0;

    grouping_start = calloc(data->longest_keyword_len + 1, sizeof(unsigned char));
    if (grouping_start == NULL) {
        return ALLOCATION_ERROR;
    }

    for (i = 0; i < data->keyword_count; i++) {
        grouping_start[data->keywords_length[i]]++;
    }

    for (i = 0; i < data->longest_keyword_len + 1; i++) {
        prev = grouping_start[i];
        grouping_start[i] = offset;
        offset += prev;
        printf("%d : %d\n", i, grouping_start[i]);
    }

    status = grouping_by_keywords_length(data, grouping_start);
    if (status != NO_ERROR) {
        return status;
    }

    return NO_ERROR;
}

Status column_selection(KeywordData *data) {
    Status status = NO_ERROR;

    status = column_grouping_by_length(data);
    if (status != NO_ERROR) {
        return status;
    }

    return NO_ERROR;
}


Status parser_start(const FileString *filestring) {
    Status status = NO_ERROR;
    MappingMode mapping_mode = KEY_ENUMERATION;
    size_t line_count = 0;

    KeywordData keyword_data = {NULL,NULL,NULL,0,0,0};

    status = header_parse(filestring, &line_count, &mapping_mode);
    if (status != NO_ERROR) {return status;}

    if (mapping_mode == KEY_VALUE_PAIR) {
        status = body_parse_key_value(filestring, &line_count, &keyword_data);
        if (status != NO_ERROR) {return status;}
    } else {
        /* i'll fill this later */
    }

/*     print_keyword_data(&keyword_data); */


    status = column_selection(&keyword_data);
    if (status != NO_ERROR) {return status;}

/*     print_keyword_data(&keyword_data); */



    return status;
}
