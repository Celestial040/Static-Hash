#ifndef LEXER_H
#define LEXER_H

#include "file_loader.h"
#include "status.h"
#include <stddef.h>
#include <stdint.h>

#include <stddef.h>

typedef uint8_t Token;
enum {
    TOKEN_IDENTIFIER,
    TOKEN_COMMA,
    TOKEN_ASSIGNMENT,
    TOKEN_OPEN_BRACKET,
    TOKEN_CLOSE_BRACKET,
    TOKEN_OPEN_CURLY_BRACKET,
    TOKEN_CLOSE_CURLY_BRACKET,
    TOKEN_NUMERIC_LITERAL,
    TOKEN_ENDLINE,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};


typedef uint8_t GenerateFields;
enum {
    STATUS_HEADER,
    UTILS_DIR,
    HEADER_DIR,
    STRUCT_TEMPLATE
};

typedef struct LookupStatus {
    Status status;
    GenerateFields fields_id;
}LookupStatus ;

typedef uint8_t TypeMode;
enum {
    TAB_SPACE,
    ENDLINE,
    ALPHANUMERIC,
    SYMBOL,
    NUMERIC_LITERAL,
    UNKNOWN_MODE
};

typedef struct TokenPointer {
    size_t start;
    size_t len;
    Token token;
} TokenPointer;

TokenPointer lexer_scan(const FileString *file_string) ;
void set_lexer_index(size_t checkpoint);
size_t get_lexer_tail(void);


#endif
