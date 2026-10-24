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
#include "grouping/selected_column.h"

typedef unsigned char MappingMode;
enum {
    KEY_ENUMERATION,
    KEY_VALUE_PAIR
};

typedef struct OptionalParseHeader {
    size_t index_position;
    size_t line_count;
    Status status;
    MappingMode mapping_mode;
}OptionalParseHeader;

typedef struct OptionalParseKeyValue {
    KeywordData data;
    Status status;
}OptionalParseKeyValue;

typedef struct OptionalValueConverter {
    Status status;
    unsigned short value;
} OptionalValueConverter;

typedef struct TextLine {
    size_t start;
    size_t length;
}TextLine;

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
}

void print_keyword_data_details(KeywordData *data) {
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

    for (i = 0; i < data->keyword_count; i++) {
        printf("%d \n", data->values_pair[i]);
    }

    printf("\n");

}

OptionalValueConverter value_converter(const FileString filestring, const size_t start, const size_t length) {
    OptionalValueConverter result = {NO_ERROR, 0};
    size_t i = 0;
    unsigned short total = 0;
    unsigned short power = 1;

    if (length >= 5) {
        result.status = MAX_LIMIT_EXCEEDED;
        return result;
    }

    for (i = length; i > 0; i--) {
        total += ((unsigned short) (filestring.start[start+i-1] - '0')) * power;
        power *= 10;
    }

    if (total > 9999) {
        result.status = MAX_LIMIT_EXCEEDED;
        return result;
    }

    result.value = total;

    return result;
}

TextLine find_next_endline(FileString filestring, size_t current_index){
    TextLine result = {0,0};
    size_t index = current_index;
    for (; index < filestring.length; index++) {
        if (filestring.start[index] == '\n') {
            break;
        }
    }
    result.length = index;
    for (index = current_index; index > 0; index--) {
        if (filestring.start[index] == '\n') {
            break;
        }
    }
    if (index == 0) {
        result.start = 0;
        return result;
    }
    result.start = index+1;
    return result;

}

OptionalParseHeader parse_header(const FileString filestring, const char *filename) {
    OptionalParseHeader result = {0,0,0,KEY_ENUMERATION};
    TextLine text_line = {NO_ERROR, 0};
    TokenPointer tokenpointer = {0,0,TOKEN_UNKNOWN};
    MappingMode mapping_mode = KEY_ENUMERATION;
    size_t index_position = 0;
    size_t line_count = 0;

    tokenpointer = lexer_scan(filestring,index_position);

/*     printf("%ld , %ld, %d \n", tokenpointer.start, tokenpointer.len, tokenpointer.token); */

    if (tokenpointer.token != TOKEN_OPEN_CURLY_BRACKET) {
        text_line = find_next_endline(filestring, index_position);
        fprintf(stderr, "%s:%ld:%ld Error '{' expected \n", filename, line_count, index_position);
        fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
        fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
        result.status = MISMATCH_EXPECTATION;
        return result;
    }

    index_position = tokenpointer.start + tokenpointer.len;

    tokenpointer = lexer_scan(filestring,index_position);

    if (tokenpointer.token != TOKEN_IDENTIFIER) {
        text_line = find_next_endline(filestring, index_position);
        fprintf(stderr, "%s:%ld:%ld Error identifier expected \n", filename, line_count, index_position);
        fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
        fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
        result.status = MISMATCH_EXPECTATION;
        return result;
    }

    if (tokenpointer.len == 4 && memcmp(filestring.start + tokenpointer.start, "enum", tokenpointer.len) == 0) {
        mapping_mode = KEY_ENUMERATION;
    } else if (tokenpointer.len == 3 && memcmp(filestring.start + tokenpointer.start, "key", tokenpointer.len) == 0) {
        mapping_mode = KEY_VALUE_PAIR;
    } else {
        text_line = find_next_endline(filestring, index_position);
        fprintf(stderr, "%s:%ld:%ld Error expected identifier 'enum' or 'key' \n", filename, line_count, index_position);
        fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
        fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
        result.status = MISMATCH_EXPECTATION;
        return result;
    }



    index_position = tokenpointer.start + tokenpointer.len;

    tokenpointer = lexer_scan(filestring,index_position);

    if (tokenpointer.token != TOKEN_CLOSE_CURLY_BRACKET) {
        text_line = find_next_endline(filestring, index_position);
        fprintf(stderr, "%s:%ld:%ld Error '}' expected \n", filename, line_count, index_position);
        fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
        fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
        result.status = MISMATCH_EXPECTATION;
        return result;
    }



    index_position = tokenpointer.start + tokenpointer.len;

    tokenpointer = lexer_scan(filestring,index_position);

    if (mapping_mode == KEY_ENUMERATION) {
        if (tokenpointer.token != TOKEN_ENDLINE) {
            text_line = find_next_endline(filestring, index_position);
            fprintf(stderr, "%s:%ld:%ld Error newline expected \n", filename, line_count, index_position);
            fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
            fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
            result.status = MISMATCH_EXPECTATION;
            return result;
        }

        line_count += 1;
        result.line_count = line_count;
        result.index_position = index_position;
        result.mapping_mode = mapping_mode;
        return result;
    }

    if (tokenpointer.token != TOKEN_COMMA) {
        text_line = find_next_endline(filestring, index_position);
        fprintf(stderr, "%s:%ld:%ld Error ',' expected \n", filename, line_count, index_position);
        fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
        fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
        result.status = MISMATCH_EXPECTATION;
        return result;
    }



    index_position = tokenpointer.start + tokenpointer.len;

    tokenpointer = lexer_scan(filestring,index_position);


    printf("%ld , %ld, %d \n", tokenpointer.start, tokenpointer.len, tokenpointer.token);


    if (tokenpointer.token != TOKEN_OPEN_CURLY_BRACKET) {
        text_line = find_next_endline(filestring, index_position);
        fprintf(stderr, "%s:%ld:%ld Error '{' expected \n", filename, line_count, index_position);
        fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
        fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
        result.status = MISMATCH_EXPECTATION;
        return result;
    }


    index_position = tokenpointer.start + tokenpointer.len;

    tokenpointer = lexer_scan(filestring,index_position);

    if (tokenpointer.token != TOKEN_IDENTIFIER) {
        text_line = find_next_endline(filestring, index_position);
        fprintf(stderr, "%s:%ld:%ld Error identifier expected \n", filename, line_count, index_position);
        fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
        fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
        result.status = MISMATCH_EXPECTATION;
        return result;
    }

    if (tokenpointer.len != 5 || memcmp(filestring.start + tokenpointer.start, "value", tokenpointer.len) != 0) {
        text_line = find_next_endline(filestring, index_position);
        fprintf(stderr, "%s:%ld:%ld Error expected identifier 'value' \n", filename, line_count, index_position);
        fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start + text_line.start);
        fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
        result.status = MISMATCH_EXPECTATION;
        return result;
    }

    index_position = tokenpointer.start + tokenpointer.len;

    tokenpointer = lexer_scan(filestring,index_position);

    if (tokenpointer.token != TOKEN_CLOSE_CURLY_BRACKET) {
        text_line = find_next_endline(filestring, index_position);
        fprintf(stderr, "%s:%ld:%ld Error '{' expected \n", filename, line_count, index_position);
        fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
        fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
        result.status = MISMATCH_EXPECTATION;
        return result;
    }

    index_position = tokenpointer.start + tokenpointer.len;

    tokenpointer = lexer_scan(filestring,index_position);

    if (tokenpointer.token != TOKEN_ENDLINE) {
        text_line = find_next_endline(filestring, index_position);
        fprintf(stderr, "%s:%ld:%ld Error newline expected \n", filename, line_count, index_position);
        fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
        fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
        result.status = MISMATCH_EXPECTATION;
        return result;
    }

    line_count += 1;

    result.line_count = line_count;
    result.index_position = index_position;
    result.mapping_mode = mapping_mode;
    return result;
}

OptionalParseKeyValue body_parse_key_value(const FileString filestring, const char *filename, const size_t start_index, const size_t start_line_count) {
    OptionalParseKeyValue result = {{NULL,NULL,NULL,0,0,0}, NO_ERROR};
    OptionalValueConverter value_convert_result = {NO_ERROR, 0};

    TextLine text_line = {NO_ERROR, 0};
    TokenPointer tokenpointer = {0,0,TOKEN_UNKNOWN};
    size_t index_position = start_index;
    size_t line_count = start_line_count;

    KeywordData data = {NULL,NULL,NULL,0,0,255};

    size_t i, j = 0;

    while (1) {
        tokenpointer = lexer_scan(filestring,index_position);

        if (data.keyword_count > 0 && tokenpointer.token == TOKEN_EOF) {
            line_count += 1;
            break;
        }

        if (tokenpointer.token != TOKEN_IDENTIFIER) {
            text_line = find_next_endline(filestring, index_position);
            fprintf(stderr, "%s:%ld:%ld Error identifier expected \n", filename, line_count, index_position);
            fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
            fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
            result.status = MISMATCH_EXPECTATION;
            return result;
        }

        if (data.keyword_count == 255) {
            text_line = find_next_endline(filestring, index_position);
            fprintf(stderr, "%s:%ld:%ld Error keyword limit exceeded (max keywords count : 255) \n", filename, line_count, index_position);
            fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
            fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
            result.status = MAX_LIMIT_EXCEEDED;
            return result;
        }

        if (tokenpointer.len > 255) {
            text_line = find_next_endline(filestring, index_position);
            fprintf(stderr, "%s:%ld:%ld Error keyword length exceeded (max keywords length : 255) \n", filename, line_count, index_position);
            fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
            fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
            result.status = STRING_TOO_LONG;
            return result;
        }

        if (tokenpointer.len > data.longest_keyword_len) {
            data.longest_keyword_len = (unsigned char) tokenpointer.len;
        }

        if (tokenpointer.len < data.shortest_keyword_len) {
            data.shortest_keyword_len = (unsigned char) tokenpointer.len;
        }

        index_position = tokenpointer.start + tokenpointer.len;

        tokenpointer = lexer_scan(filestring,index_position);

        if (tokenpointer.token != TOKEN_COMMA) {
            text_line = find_next_endline(filestring, index_position);
            fprintf(stderr, "%s:%ld:%ld Error ',' expected \n", filename, line_count, index_position);
            fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
            fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
            result.status = MISMATCH_EXPECTATION;
            return result;
        }

        index_position = tokenpointer.start + tokenpointer.len;

        tokenpointer = lexer_scan(filestring,index_position);

        if (tokenpointer.token != TOKEN_NUMERIC_LITERAL) {
            text_line = find_next_endline(filestring, index_position);
            fprintf(stderr, "%s:%ld:%ld Error numeric value expected \n", filename, line_count, index_position);
            fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
            fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
            result.status = MISMATCH_EXPECTATION;
            return result;
        }

        index_position = tokenpointer.start + tokenpointer.len;

        tokenpointer = lexer_scan(filestring,index_position);

        if (tokenpointer.token != TOKEN_ENDLINE && tokenpointer.token != TOKEN_EOF) {
            text_line = find_next_endline(filestring, index_position);
            fprintf(stderr, "%s:%ld:%ld Error newline expected \n", filename, line_count, index_position);
            fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
            fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
            result.status = MISMATCH_EXPECTATION;
            return result;
        }

        index_position = tokenpointer.start + tokenpointer.len;
        line_count += 1;
        data.keyword_count++;
    }

    data.keywords = (char *) calloc(data.keyword_count * data.longest_keyword_len , sizeof(char));
    if (data.keywords == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    data.keywords_length = (unsigned char *) calloc(data.keyword_count , sizeof(char));
    if (data.keywords_length == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    data.values_pair = (unsigned short *) malloc(sizeof(unsigned short) * data.keyword_count);
    if (data.values_pair == NULL ) {
        result.status = ALLOCATION_ERROR;
        return result;
    }


    index_position = start_index;
    line_count = start_line_count;

    for (i = 0; i < data.keyword_count; i++) {
        tokenpointer = lexer_scan(filestring,index_position);

        data.keywords_length[i] = (unsigned char) tokenpointer.len;

        for (j = 0; j < tokenpointer.len; j++) {
            data.keywords[i * data.longest_keyword_len + j] =  filestring.start[tokenpointer.start + j];
        }

        index_position = tokenpointer.start + tokenpointer.len;
        tokenpointer = lexer_scan(filestring,index_position);

        index_position = tokenpointer.start + tokenpointer.len;
        tokenpointer = lexer_scan(filestring,index_position);

        value_convert_result = value_converter(filestring, tokenpointer.start, tokenpointer.len);
        if (value_convert_result.status == DIGITS_TOO_LONG) {
            fprintf(stderr, "%s:%ld:%ld Error max value limit excedeed (max value: 9999) \n", filename, line_count, index_position);
            fprintf(stderr, "|   %.*s\n", (int)(text_line.length), filestring.start+ text_line.start);
            fprintf(stderr, "|   %*s^\n", (int)(index_position - text_line.start), "");
            result.status = MISMATCH_EXPECTATION;
            return result;
        }

        data.values_pair[i] = value_convert_result.value;

        index_position = tokenpointer.start + tokenpointer.len;
        tokenpointer = lexer_scan(filestring,index_position);

        index_position = tokenpointer.start + tokenpointer.len;
        line_count++;
    }

    result.data = data;
    return result;
}

/* Status row_grouping_by_length(KeywordData *data, unsigned char *grouping_count_array, unsigned char *grouping_index_start) {
    Status status = NO_ERROR;

    unsigned char i = 0;
    unsigned char *length_count = NULL;
    unsigned char *grouping_start = NULL;
    unsigned char prev = 0;
    unsigned char offset = 0;

    length_count = (unsigned char *) calloc(data->longest_keyword_len + 1, sizeof(unsigned char));
    if (length_count == NULL) {
        return ALLOCATION_ERROR;
    }

    grouping_start = (unsigned char *) calloc(data->longest_keyword_len + 1, sizeof(unsigned char));
    if (grouping_start == NULL) {
        return ALLOCATION_ERROR;
    }

    for (i = 0; i < data->keyword_count; i++) {
        length_count[data->keywords_length[i]]++;
    }

    for (i = 0; i < data->longest_keyword_len + 1; i++) {
        prev = length_count[i];
        grouping_start[i] = offset;
        offset += prev;
    }

    memcpy(grouping_count_array, length_count, data->longest_keyword_len + 1);
    memcpy(grouping_index_start, grouping_start, data->longest_keyword_len + 1);

    status = grouping_by_keywords_length(data, grouping_start);
    if (status != NO_ERROR) {
        return status;
    }

    free(length_count);
    free(grouping_start);

    return NO_ERROR;
}

Status row_grouping_by_selected_column(KeywordData *data, unsigned char selected_column , unsigned char *grouping_count_array, unsigned char *grouping_index_start) {
    Status status = NO_ERROR;

    unsigned char i,j = 0;
    unsigned char *grouping_count = NULL;
    unsigned char *grouping_start = NULL;
    unsigned char insert_count = 0;

    unsigned char prev = 0;
    unsigned char offset = 0;

    unsigned char uppercase[26] = {0};
    unsigned char lowercase[26] = {0};
    unsigned char uppercase_start[26] = {0};
    unsigned char lowercase_start[26] = {0};

    char selected_char = 0;

    grouping_count = (unsigned char *) calloc(data->longest_keyword_len + 1, sizeof(unsigned char));
    if (grouping_count == NULL) {
        return ALLOCATION_ERROR;
    }

    grouping_start = (unsigned char *) calloc(data->longest_keyword_len + 1, sizeof(unsigned char));
    if (grouping_start == NULL) {
        return ALLOCATION_ERROR;
    }


    for (i = data->keywords_length[0]; i < data->longest_keyword_len + 1; i++) {
        for (j = grouping_index_start[i]; j < grouping_index_start[i] + grouping_count_array[i]; j++) {
            selected_char = data->keywords[j * data->longest_keyword_len + selected_column];
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
        }
        for (j = 0; j < 26; j++) {
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
                grouping_count[insert_count] = lowercase[j];
                grouping_start[insert_count] = lowercase_start[j];
                insert_count++;
            }
        }

        for (j = 0; j < 26; j++) {
            if (uppercase[j] > 0) {
                printf("%c = %d ",'A' + j, uppercase[j]);
                printf("%c = %d\n",'A' + j, uppercase_start[j]);
                grouping_count[insert_count] = uppercase[j];
                grouping_start[insert_count] = uppercase_start[j];
                insert_count++;
            }
        }

        if (grouping_count_array[i] > 1) {
            status = grouping_by_selected_column(data, selected_column, uppercase_start, lowercase_start, grouping_index_start[i], grouping_count_array[i]);
            if (status != NO_ERROR) {
                return status;
            }
        }

        print_keyword_data(data);

        prev = 0;
        offset = 0;

        for (j = 0; j < 26; j++) {
            lowercase[j] = 0;
            lowercase_start[j] = 0;
            uppercase[j] = 0;
            uppercase_start[j] = 0;
        }

    }

    for (i = 0; i < data->longest_keyword_len + 1; i++) {
        printf("%d \n", grouping_count[i]);
    }

    return NO_ERROR;
}

Status column_selection(KeywordData *data) {
    Status status = NO_ERROR;

    unsigned char i,j,k = 0;
    unsigned char shortest_keyword_length = 0;

    unsigned char uppercase[26] = {0};
    unsigned char lowercase[26] = {0};

    unsigned char *grouping_count_array = NULL;
    unsigned char *grouping_index_start = NULL;

    unsigned short *column_separation_score = NULL;
    unsigned char *selected_columns = NULL;

    unsigned char selected_columns_count = 0;
    unsigned char current_best_column = 0;

    unsigned short temp_total = 0;

    char selected_char = 0;

    grouping_count_array = (unsigned char *) calloc(data->longest_keyword_len + 1, sizeof(unsigned char));
    if (grouping_count_array == NULL) {
        return ALLOCATION_ERROR;
    }

    grouping_index_start = (unsigned char *) calloc(data->longest_keyword_len + 1, sizeof(unsigned char));
    if (grouping_index_start == NULL) {
        return ALLOCATION_ERROR;
    }

    status = row_grouping_by_length(data, grouping_count_array, grouping_index_start);
    if (status != NO_ERROR) {
        return status;
    }

    shortest_keyword_length = data->keywords_length[0];

    selected_columns = (unsigned char *) malloc(sizeof(unsigned char) * data->longest_keyword_len);
    if (selected_columns == NULL) {
        return ALLOCATION_ERROR;
    }

    column_separation_score = (unsigned short *) calloc(data->longest_keyword_len, sizeof(unsigned short));
    if (column_separation_score == NULL) {
        return ALLOCATION_ERROR;
    }

    for (i = shortest_keyword_length; i < data->longest_keyword_len + 1; i++) {
        temp_total = grouping_count_array[i] * (grouping_count_array[i] - 1) / 2;
        for (j = 0; j < i; j++) {
            column_separation_score[j] += temp_total;
        }

        for (j = 0; j < i; j++) {
            for (k = grouping_index_start[i]; k < grouping_index_start[i]+grouping_count_array[i] ; k++) {
                selected_char = data->keywords[k * data->longest_keyword_len + j];
                if (is_lowercase_alphabet(selected_char) == 1) {
                    lowercase[selected_char - 'a']++;
                } else {
                    uppercase[selected_char - 'A']++;
                }
            }

            for (k = 0; k < 26; k++) {
                if (lowercase[k] > 1) {
                    column_separation_score[j] -= lowercase[k] * (lowercase[k] - 1) / 2;
                }

                if (uppercase[k] > 1) {
                    column_separation_score[j] -= uppercase[k] * (uppercase[k] - 1) / 2;
                }

                lowercase[k] = 0;
                uppercase[k] = 0;
            }
        }
    }

    for (j = 0; j < data->longest_keyword_len; j++) {
        if (column_separation_score[j] > column_separation_score[current_best_column]) {
            current_best_column = j;
        }
    }

    selected_columns[selected_columns_count] = current_best_column;
    selected_columns_count++;



    status = row_grouping_by_selected_column(data, current_best_column, grouping_count_array, grouping_index_start);
    if (status != NO_ERROR) {
        return status;
    }

    return NO_ERROR;
} */


OptionalParser parser_start(const FileString filestring, const char *filename) {
    OptionalParser result = {NO_ERROR};
    OptionalParseHeader parse_header_result = {0,0,0,KEY_ENUMERATION};
    OptionalParseKeyValue parse_key_value_result = {{NULL,NULL,NULL,0,0,0}, NO_ERROR};

    parse_header_result = parse_header(filestring, filename);
    if (parse_header_result.status != NO_ERROR) {
        result.status = parse_header_result.status;
        return result;
    }

    if (parse_header_result.mapping_mode == KEY_VALUE_PAIR) {
        parse_key_value_result = body_parse_key_value(filestring, filename, parse_header_result.index_position, parse_header_result.line_count);
        if (parse_key_value_result.status != NO_ERROR) {
            result.status = parse_key_value_result.status;
            return result;
        }
    } else if (parse_header_result.mapping_mode == KEY_ENUMERATION) {

    }

    print_keyword_data_details(&parse_key_value_result.data);

/*     status = column_selection(&keyword_data);
    if (status != NO_ERROR) {return status;} */

    return result;
}
