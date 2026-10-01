#include "parser.h"
#include "file_loader.h"
#include "lexer.h"
#include "status.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

    tokenpointer = lexer_scan(filestring, line_count);

    if (tokenpointer.token != TOKEN_OPEN_CURLY_BRACKET) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring, line_count);

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


    tokenpointer = lexer_scan(filestring, line_count);

    if (tokenpointer.token != TOKEN_CLOSE_CURLY_BRACKET) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring, line_count);


    if (tokenpointer.token != TOKEN_COMMA && tokenpointer.token != TOKEN_ENDLINE) {
        return MISMATCH_EXPECTATION;
    }

    if (headers_loop == 1 && tokenpointer.token == TOKEN_COMMA) {
        *mapping_mode = KEY_VALUE_PAIR;
    } else {
        *mapping_mode = KEY_ENUMERATION;
        return NO_ERROR;
    }

    tokenpointer = lexer_scan(filestring, line_count);

    if (tokenpointer.token != TOKEN_OPEN_CURLY_BRACKET) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring, line_count);

    if (tokenpointer.token != TOKEN_IDENTIFIER) {
        return MISMATCH_EXPECTATION;
    }

    if (strncmp(filestring->start + tokenpointer.start, expected_headers[2], tokenpointer.len) != 0) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring, line_count);

    if (tokenpointer.token != TOKEN_CLOSE_CURLY_BRACKET) {
        return MISMATCH_EXPECTATION;
    }

    tokenpointer = lexer_scan(filestring, line_count);

    if (tokenpointer.token != TOKEN_ENDLINE) {
        return MISMATCH_EXPECTATION;
    }



    return NO_ERROR;
}

Status body_parse_key_value(const FileString *filestring, size_t *line_count) {
    TokenPointer tokenpointer;
    size_t body_start_index = get_lexer_tail();
    printf("%c \n", filestring->start[body_start_index]);


    while (1) {
        tokenpointer = lexer_scan(filestring, line_count);

        if (tokenpointer.token == TOKEN_EOF) {
            break;
        }

        if (tokenpointer.token != TOKEN_IDENTIFIER) {
            return MISMATCH_EXPECTATION;
        }

        printf("%.*s \n",(unsigned int)tokenpointer.len, filestring->start+tokenpointer.start);

        tokenpointer = lexer_scan(filestring, line_count);

        if (tokenpointer.token == TOKEN_EOF) {
            break;
        }

        if (tokenpointer.token != TOKEN_COMMA) {
            return MISMATCH_EXPECTATION;
        }

        tokenpointer = lexer_scan(filestring, line_count);

        if (tokenpointer.token == TOKEN_EOF) {
            break;
        }

        if (tokenpointer.token != TOKEN_NUMERIC_LITERAL) {
            return MISMATCH_EXPECTATION;
        }

        printf("%.*s \n",(unsigned int)tokenpointer.len, filestring->start+tokenpointer.start);

        tokenpointer = lexer_scan(filestring, line_count);

        if (tokenpointer.token == TOKEN_EOF) {
            break;
        }

        if (tokenpointer.token != TOKEN_ENDLINE) {
            return MISMATCH_EXPECTATION;
        }
    }

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
