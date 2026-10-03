#include <stddef.h>
#include <stdint.h>
#include "status.h"

unsigned char is_uppercase_alphabet(const char target) {
    return target >= 65 && target <= 90;
}

unsigned char is_lowercase_alphabet(const char target) {
    return target >= 97 && target <= 122;
}

unsigned char is_alphabet(const char target) {
    return is_uppercase_alphabet(target) || is_lowercase_alphabet(target);
}

unsigned char is_numeric(const char target) {
    return target >= 48 && target <= 57;
}

unsigned char is_alphabet_numeric(const char target) {
    return is_alphabet(target) || is_numeric(target);
}

unsigned char is_tab_space(const char target) {
    return target == ' ' || target == 9 ||(target > '\n' && target <= '\r');
}

unsigned char is_endline(const char target) {
    return target == '\n';
}

unsigned char is_whitespace(const char target) {
    return is_tab_space(target)||is_endline(target);
}

Status string_to_unsigned_short(const char *src, const size_t src_len, unsigned short *output) {
    size_t i = 0;
    unsigned short total = 0;
    unsigned short power = 1;

    if (src_len >= 5) {
        return DIGITS_TOO_LONG;
    }

    for (i = src_len; i > 0; i--) {
        total += ((unsigned short) (src[i-1] - '0')) * power;
        power *= 10;
    }

    if (total > 9999) {
        return MAX_LIMIT_EXCEEDED;
    }

    *output = total;

    return NO_ERROR;
}
