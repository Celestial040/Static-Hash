#include "parser.h"
#include "file_loader.h"
#include "lexer.h"
#include "status.h"
#include "char_manip.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

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
    Keyword_Data data;
    Status status;
}Parse_KeyValue;

Parse_Header parse_header(
    const FileString filestring,
    const char *filename
) {
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
    Parse_KeyValue result = {
        {NULL, NULL, NULL, 0, 0, 0}, NO_ERROR
    };

    Value_Converter value_convert_result = {
        NO_ERROR, 0
    };

    TextLine text_line = {
        NO_ERROR, 0
    };

    TokenPointer tokenpointer = {
        0, 0, TOKEN_UNKNOWN
    };

    size_t index_position = start_index;
    size_t line_count = start_line_count;

    Keyword_Data_Status new_data = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0, 0}
    };

    unsigned char keyword_count = 0;
    unsigned char longest_keyword = 0;
    unsigned char shortest_keyword = 255;
    size_t i, j = 0;

    while (1) {
        tokenpointer = lexer_scan(filestring,index_position);

        if (keyword_count > 0 && tokenpointer.token == TOKEN_EOF) {
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

        if (keyword_count >= 255) {
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

        if (tokenpointer.len > longest_keyword) {
            longest_keyword = (unsigned char) tokenpointer.len;
        }

        if (tokenpointer.len < shortest_keyword) {
            shortest_keyword = (unsigned char) tokenpointer.len;
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
        keyword_count++;
    }

    new_data = allocate_keyword_data(
        keyword_count,
        shortest_keyword,
        longest_keyword
    );

    if (new_data.status != NO_ERROR) {
        result.status = new_data.status;
        return result;
    }

    index_position = start_index;
    line_count = start_line_count;

    for (i = 0; i < new_data.data.keyword_count; i++) {
        tokenpointer = lexer_scan(filestring,index_position);

        new_data.data.keywords_length[i] = (unsigned char) tokenpointer.len;

        for (j = 0; j < tokenpointer.len; j++) {
            new_data.data.keywords[i * new_data.data.longest_keyword_len + j] = filestring.start[tokenpointer.start + j];
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

        new_data.data.values_pair[i] = value_convert_result.value;

        index_position = tokenpointer.start + tokenpointer.len;
        tokenpointer = lexer_scan(filestring,index_position);

        index_position = tokenpointer.start + tokenpointer.len;
        line_count++;
    }

    result.data = new_data.data;
    return result;
}

Parser parser_start(
    const FileString filestring,
    const char *filename
) {
    Parser result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0, 0}
    };

    Parse_Header parse_header_result = {
        0, 0, 0, KEY_ENUMERATION
    };

    Parse_KeyValue parse_key_value_result = {
        {NULL, NULL, NULL, 0, 0, 0}, NO_ERROR
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
    }

    else if (parse_header_result.mapping_mode == KEY_ENUMERATION) {

    }

    result.data = parse_key_value_result.data;
    return result;
}
