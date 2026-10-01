#ifndef CHAR_MANIP_H
#define CHAR_MANIP_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct String {
    char *buffer;
    size_t length;
} String ;

unsigned char is_uppercase_alphabet(const char target);
unsigned char is_lowercase_alphabet(const char target);
unsigned char is_alphabet(const char target);
unsigned char is_numeric(const char target);
unsigned char is_alphabet_numeric(const char target);
unsigned char is_tab_space(const char target);
unsigned char is_endline(const char target);
unsigned char is_whitespace(const char target);


#endif
