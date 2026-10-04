#include "parser.h"
#include "file_loader.h"
#include "lexer.h"
#include "status.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "char_manip.h"


typedef unsigned char MappingMode;
enum {
    KEY_ENUMERATION,
    KEY_VALUE_PAIR,
    STRUCT_MAPPING
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



Status body_parse_key_value(const FileString *filestring, size_t *line_count) {
    Status status = NO_ERROR;
    TokenPointer tokenpointer;
    unsigned char *keyword_column = NULL;
    unsigned short *values_pair = NULL;
    size_t body_start_index = get_lexer_tail();
    unsigned char longest_keyword_len = 0;
    size_t keyword_count = 0;
    size_t i , j;
    Keyword *keyword_collections = NULL;

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

    set_lexer_index(body_start_index);

    keyword_collections = (Keyword *) malloc(sizeof(Keyword) * keyword_count);
    if (keyword_collections == NULL) { return ALLOCATION_ERROR; }

    keyword_column = (unsigned char *) calloc(keyword_count * longest_keyword_len , sizeof(char));
    if (keyword_column == NULL) { return ALLOCATION_ERROR; }

    values_pair = (unsigned short *) malloc(sizeof(unsigned short) * keyword_count);
    if (values_pair == NULL) { return ALLOCATION_ERROR; }

    for (i = 0; i < keyword_count; i++) {

        /* keyword */
        tokenpointer = lexer_scan(filestring);

        keyword_collections[i].start = tokenpointer.start;
        keyword_collections[i].len = tokenpointer.len;

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

    /* Important note here
       I will add pure copy paste for non int value pairs in the future
       but to simply for now, im gonna use easy simple way of num as value
    */

    /* for (i = 0; i < keyword_count; i++) {
        printf("%.*s \n", (unsigned int) keyword_collections[i].len, filestring->start + keyword_collections[i].start);
    } */

    /* for (i = 0; i < keyword_count; i++) {

        for (j = i * keyword_count; j < (i + 1) * keyword_count; j++) {

            if (keyword_column[j] != 0) {
                printf("%c ", keyword_column[j]);
            }

            else {
                printf("  ");
            }
        }

        printf("\n");
    }

    for (i = 0; i < keyword_count; i++) {
        printf("value : %d\n", values_pair[i]);
    } */




    return NO_ERROR;
}

Status parser_start(const FileString *filestring) {
    size_t line_count = 0;
    MappingMode mapping_mode = KEY_ENUMERATION;
    Status status = NO_ERROR;

    status = header_parse(filestring, &line_count, &mapping_mode);
    if (status != NO_ERROR) {return status;}


    switch (mapping_mode) {
        case KEY_VALUE_PAIR:
            status = body_parse_key_value(filestring, &line_count);
            if (status != NO_ERROR) {return status;}
            break;

        default:
            break;
    }

    return status;
}
