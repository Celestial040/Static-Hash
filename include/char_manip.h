#ifndef CHAR_MANIP_H
#define CHAR_MANIP_H

#include "status.h"
#include "file_loader.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct String {
    char *buffer;
    size_t length;
} String;

typedef struct Value_Converter {
    Status status;
    unsigned short value;
} Value_Converter;

typedef struct TextLine {
    size_t start;
    size_t length;
}TextLine;

unsigned char is_uppercase_alphabet(const char target);
unsigned char is_lowercase_alphabet(const char target);
unsigned char is_alphabet(const char target);
unsigned char is_numeric(const char target);
unsigned char is_alphabet_numeric(const char target);
unsigned char is_tab_space(const char target);
unsigned char is_endline(const char target);
unsigned char is_whitespace(const char target);

Value_Converter value_converter(
    const FileString filestring,
    const size_t start,
    const size_t length
);

TextLine find_next_endline(
    const FileString filestring,
    const size_t current_index
);

enum {
    FALSE = 0,
    TRUE = 1
};

#endif
