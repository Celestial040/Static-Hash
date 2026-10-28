#include "parser.h"
#include "file_loader.h"
#include "lexer.h"
#include "status.h"
#include "char_manip.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grouping/keywords_length.h"
#include "grouping/selected_column.h"

typedef unsigned char MappingMode;
enum {
    KEY_ENUMERATION,
    KEY_VALUE_PAIR
};

typedef struct Parse_Header {
    size_t index_position;
    size_t line_count;
    Status status;
    MappingMode mapping_mode;
}Parse_Header;

typedef struct Parse_KeyValue {
    KeywordData data;
    Status status;
}Parse_KeyValue;

typedef struct Value_Converter {
    Status status;
    unsigned short value;
} Value_Converter;

typedef struct TextLine {
    size_t start;
    size_t length;
}TextLine;

typedef struct Column_Selection {
    Status status;
    KeywordData data;
} Column_Selection;

typedef struct Grouping_ByLength {
    Status status;
    KeywordData data;
    unsigned char *grouping_count;
    unsigned char *grouping_start_index;
} Grouping_ByLength;

typedef struct Internal_Column_Selection {
    Status status;
    unsigned char selected_column;
}Internal_Column_Selection;

typedef struct Grouping_BySelectedColumn {
    Status status;
    KeywordData data;
    unsigned char *grouping_count;
    unsigned char *grouping_start_index;
} Grouping_BySelectedColumn;

void free_keyword_data(KeywordData *data) {
    free(data->keywords);
    free(data->keywords_length);
    free(data->values_pair);

    data->keywords = NULL;
    data->keywords_length = NULL;
    data->values_pair = NULL;

    data->keyword_count = 0;
    data->shortest_keyword_len = 0;
    data->longest_keyword_len = 255;
}

void print_keyword_data(const KeywordData data) {
    unsigned char i = 0;

    for (i = 0; i < data.keyword_count; i++) {
        printf("%.*s \n", data.keywords_length[i], data.keywords + i * data.longest_keyword_len);
    }

    printf("\n");
}

void print_keyword_data_details(const KeywordData data) {
    unsigned char i = 0;

    for (i = 0; i < data.keyword_count; i++) {
        printf("%.*s \n", data.keywords_length[i], data.keywords + i * data.longest_keyword_len);
    }

    printf("\n");

    for (i = 0; i < data.keyword_count; i++) {
        printf("%d \n", data.keywords_length[i]);
    }

    printf("\n");

    for (i = 0; i < data.keyword_count; i++) {
        printf("%d \n", data.values_pair[i]);
    }

    printf("\n");

}

Value_Converter value_converter(
    const FileString filestring,
    const size_t start,
    const size_t length
) {
    Value_Converter result = {NO_ERROR, 0};
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

TextLine find_next_endline(const FileString filestring, const size_t current_index){
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

Parse_Header parse_header(const FileString filestring, const char *filename) {
    Parse_Header result = {0,0,0,KEY_ENUMERATION};
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

    index_position = tokenpointer.start + tokenpointer.len;

    line_count += 1;

    result.line_count = line_count;
    result.index_position = index_position;
    result.mapping_mode = mapping_mode;
    return result;
}

Parse_KeyValue body_parse_key_value(
    const FileString filestring,
    const char *filename,
    const size_t start_index,
    const size_t start_line_count
) {
    Parse_KeyValue result = {{NULL,NULL,NULL,0,0,0}, NO_ERROR};
    Value_Converter value_convert_result = {NO_ERROR, 0};

    TextLine text_line = {NO_ERROR, 0};
    TokenPointer tokenpointer = {0,0,TOKEN_UNKNOWN};
    size_t index_position = start_index;
    size_t line_count = start_line_count;

    KeywordData data = {NULL,NULL,NULL,0,0,255};

    size_t i, j = 0;

    while (1) {
        tokenpointer = lexer_scan(filestring,index_position);

        if (data.keyword_count > 0 && tokenpointer.token == TOKEN_EOF) {
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

        if (data.keyword_count >= 255) {
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
            data.keywords[i * data.longest_keyword_len + j] = filestring.start[tokenpointer.start + j];
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

Grouping_ByLength row_grouping_by_length(const KeywordData data) {
    Grouping_ByLength result = {
        NO_ERROR,
        {NULL, NULL, NULL, 0, 0 ,0},
        NULL, NULL
    };

    Internal_Grouping_ByLength internal_result = {NO_ERROR,
        {NULL, NULL, NULL, 0, 0 ,0}
    };

    unsigned char i = 0;
    unsigned char *grouping_count = NULL;
    unsigned char *grouping_start = NULL;
    unsigned char prev = 0;
    unsigned char offset = 0;

    grouping_count = (unsigned char *) calloc(data.longest_keyword_len + 1, sizeof(unsigned char));
    if (grouping_count == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    grouping_start = (unsigned char *) calloc(data.longest_keyword_len + 1, sizeof(unsigned char));
    if (grouping_start == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    for (i = 0; i < data.keyword_count; i++) {
        grouping_count[data.keywords_length[i]]++;
    }

    for (i = 0; i < data.longest_keyword_len + 1; i++) {
        prev = grouping_count[i];
        grouping_start[i] = offset;
        offset += prev;
    }

    internal_result = grouping_by_keywords_length(data, grouping_start);
    if (internal_result.status != NO_ERROR) {
        result.status = internal_result.status;
        return result;
    }

    result.data = internal_result.data;
    result.grouping_count = grouping_count;
    result.grouping_start_index = grouping_start;
    return result;
}

Grouping_BySelectedColumn row_grouping_by_selected_column(
    const KeywordData data,
    const unsigned char selected_column,
    const unsigned char *grouping_count,
    const unsigned char *grouping_start
) {
    Grouping_BySelectedColumn result = {
        NO_ERROR,
        {NULL,NULL,NULL,0,0,0}
        ,NULL
        ,NULL
    };

    KeywordData new_data = {
        NULL,
        NULL,
        NULL,
        0,
        0,
        0
    };

    Internal_Grouping_BySelectedColumn internal_result = {
        NO_ERROR,
        NULL,
        NULL,
        NULL
    };

    unsigned char i,j = 0;

    unsigned char prev = 0;
    unsigned char offset = 0;

    unsigned char uppercase[26] = {0};
    unsigned char lowercase[26] = {0};

    unsigned char uppercase_start[26] = {0};
    unsigned char lowercase_start[26] = {0};

    unsigned char *new_grouping_count = NULL;
    unsigned char *new_grouping_start = NULL;
    unsigned char insertion_count = 0;

    char selected_char = 0;

    new_data.keyword_count = data.keyword_count;
    new_data.longest_keyword_len = data.longest_keyword_len;
    new_data.shortest_keyword_len = data.shortest_keyword_len;

    new_data.keywords = (char *) calloc(data.keyword_count * data.longest_keyword_len,sizeof(char));
    if (new_data.keywords == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    new_data.keywords_length = (unsigned char *) malloc(sizeof(unsigned char) * data.keyword_count);
    if (new_data.keywords_length == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    new_data.values_pair = (unsigned short *) malloc(sizeof(unsigned char) * data.keyword_count);
    if (new_data.values_pair == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    memcpy(
        new_data.keywords,
        data.keywords,
        data.longest_keyword_len * data.keyword_count
    );

    memcpy(new_data.keywords_length,
        data.keywords_length,
        data.keyword_count
    );

    memcpy(new_data.values_pair,
        data.values_pair,
        data.keyword_count
    );

    new_grouping_count = (unsigned char *) malloc(sizeof(unsigned char) * data.keyword_count);
    if (new_grouping_count == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    new_grouping_start = (unsigned char *) malloc(sizeof(unsigned char) * data.keyword_count);
    if (new_grouping_start == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    for (i = data.shortest_keyword_len; i < data.longest_keyword_len + 1; i++) {

        for (j = grouping_start[i]; j < grouping_start[i] + grouping_count[i]; j++) {
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
/*                 printf("%c = %d ", 'a' + j, lowercase[j]);
                printf("%c = %d\n", 'a' + j, lowercase_start[j]); */
                new_grouping_count[insertion_count] = lowercase[j];
                new_grouping_start[insertion_count] = lowercase_start[j];
                if (insertion_count > 0) {
                    new_grouping_start[insertion_count] += new_grouping_start[insertion_count - 1];
                }
                insertion_count++;
            }
        }

        for (j = 0; j < 26; j++) {
            if (uppercase[j] > 0) {
/*                 printf("%c = %d ",'A' + j, uppercase[j]);
                printf("%c = %d\n",'A' + j, uppercase_start[j]); */
                new_grouping_count[insertion_count] = uppercase[j];
                new_grouping_start[insertion_count] = uppercase_start[j];
                if (insertion_count > 0) {
                    new_grouping_start[insertion_count] += new_grouping_start[insertion_count - 1];
                }
                insertion_count++;
            }
        }

        if (grouping_count[i] > 1) {
            internal_result = grouping_by_selected_column (
                data,
                selected_column,
                uppercase_start,
                lowercase_start,
                grouping_start[i],
                grouping_count[i]
            );

            if (internal_result.status != NO_ERROR) {
                result.status = internal_result.status;
                return result;
            }
        }

        memcpy(
            new_data.keywords + grouping_start[i] * new_data.longest_keyword_len,
            internal_result.keywords,
            grouping_count[i] * new_data.longest_keyword_len
        );

        memcpy(
            new_data.keywords_length + grouping_start[i],
            internal_result.keywords_length,
            grouping_count[i]
        );

        memcpy(
            new_data.values_pair + grouping_start[i],
            internal_result.values_pair,
            grouping_count[i]
        );

        memset(lowercase, 0, 26);
        memset(lowercase_start, 0, 26);
        memset(uppercase, 0, 26);
        memset(uppercase_start, 0, 26);

        prev = 0;
        offset = 0;

    }

    result.data = new_data;
    result.grouping_count = new_grouping_count;
    result.grouping_start_index = new_grouping_start;

    return result;
}

Internal_Column_Selection internal_column_selection_length (
    const KeywordData data,
    const unsigned char *group_count,
    const unsigned char *group_start
) {
    Internal_Column_Selection result = {NO_ERROR, 0};

    unsigned char uppercase[26] = {0};
    unsigned char lowercase[26] = {0};

    unsigned char current_best_column = 0;

    unsigned char *column_separation_score = NULL;

    unsigned short total_combination = 0;
    char selected_char = 0;
    unsigned char i,j,k = 0;

    column_separation_score = (unsigned char *) calloc(data.longest_keyword_len, sizeof(unsigned char));
    if (column_separation_score == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    for (i = data.shortest_keyword_len; i < data.longest_keyword_len + 1; i++) {
            total_combination = group_count[i] * (group_count[i] - 1) / 2;
            for (j = 0; j < i; j++) {
                column_separation_score[j] += total_combination;
            }

            for (j = 0; j < i; j++) {
                for (k = group_start[i]; k < group_start[i] + group_count[i] ; k++) {
                    selected_char = data.keywords[k * data.longest_keyword_len + j];
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

        for (j = 0; j < data.longest_keyword_len; j++) {
            if (column_separation_score[j] > column_separation_score[current_best_column]) {
                current_best_column = j;
            }

        }


    result.selected_column = current_best_column;
    return result;
}

Column_Selection column_selection_base_on_length(const KeywordData data) {
    Column_Selection result = {
        NO_ERROR,
        {NULL, NULL, NULL, 0, 0 ,0}
    };

    Grouping_ByLength length_grouping_result = {
        NO_ERROR,
        {NULL, NULL, NULL, 0, 0 ,0},
        NULL,
        NULL
    };

    Internal_Column_Selection selection_result = {NO_ERROR, 0};

    Grouping_BySelectedColumn selected_column_grouping_result = {
        NO_ERROR,
        {NULL,NULL,NULL,0,0,0},
        NULL,
        NULL
    };

    unsigned char i = 0;

    length_grouping_result = row_grouping_by_length(data);
    if (length_grouping_result.status != NO_ERROR) {
        result.status = length_grouping_result.status;
        return result;
    }

    /* print_keyword_data_details(length_grouping_result.data); */

    selection_result = internal_column_selection_length(
        length_grouping_result.data,
        length_grouping_result.grouping_count,
        length_grouping_result.grouping_start_index
    );

    if (selection_result.status != NO_ERROR) {
        result.status = selection_result.status;
        return result;
    }

/*     printf("%d \n \n", selection_result.selected_column); */

    selected_column_grouping_result = row_grouping_by_selected_column(
        length_grouping_result.data,
        selection_result.selected_column,
        length_grouping_result.grouping_count,
        length_grouping_result.grouping_start_index
    );

    if (selected_column_grouping_result.status != NO_ERROR) {
        result.status = selection_result.status;
        return result;
    }

    print_keyword_data_details(selected_column_grouping_result.data);
    for (i = 0; i < selected_column_grouping_result.data.keyword_count; i++) {

        printf(
            "%d, %d\n",
            selected_column_grouping_result.grouping_count[i],
            selected_column_grouping_result.grouping_start_index[i]
        );

    }

    return result;
}

Parser parser_start(const FileString filestring, const char *filename) {
    Parser result = {NO_ERROR};
    Parse_Header parse_header_result = {0,0,0,KEY_ENUMERATION};

    Parse_KeyValue parse_key_value_result = {
        {NULL,NULL,NULL,0,0,0},
        NO_ERROR
    };

    Column_Selection column_selection_result = {
        NO_ERROR,
        {NULL, NULL, NULL, 0, 0 ,0}
    };

    parse_header_result = parse_header(filestring, filename);
    if (parse_header_result.status != NO_ERROR) {
        result.status = parse_header_result.status;
        return result;
    }

    if (parse_header_result.mapping_mode == KEY_VALUE_PAIR) {

        parse_key_value_result = body_parse_key_value(
            filestring,
            filename,
            parse_header_result.index_position,
            parse_header_result.line_count
        );

        if (parse_key_value_result.status != NO_ERROR) {
            result.status = parse_key_value_result.status;
            return result;
        }
    } else if (parse_header_result.mapping_mode == KEY_ENUMERATION) {

    }

    column_selection_result = column_selection_base_on_length(parse_key_value_result.data);
    if (column_selection_result.status != NO_ERROR) {
        result.status = column_selection_result.status;
        return result;
    }

    free_keyword_data(&parse_key_value_result.data);


    return result;
}
