#include <stddef.h>
#include <stdint.h>

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
