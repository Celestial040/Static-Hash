#include <stddef.h>
#include <stdint.h>
#include "status.h"
#include "char_manip.h"
#include "file_loader.h"

unsigned char is_uppercase_alphabet(const char target) {
    return target >= 65 && target <= 90;
}

unsigned char is_lowercase_alphabet(const char target) {
    return target >= 97 && target <= 122;
}

unsigned char is_alphabet(const char target) {
    return
    is_uppercase_alphabet(target)
    ||is_lowercase_alphabet(target);
}

unsigned char is_numeric(const char target) {
    return target >= 48 && target <= 57;
}

unsigned char is_alphabet_numeric(const char target) {
    return is_alphabet(target) || is_numeric(target);
}

unsigned char is_tab_space(const char target) {
    return
    target == ' ' ||
    target == 9 ||
    (target > '\n' && target <= '\r');
}

unsigned char is_endline(const char target) {
    return target == '\n';
}

unsigned char is_whitespace(const char target) {
    return is_tab_space(target)||is_endline(target);
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

TextLine find_next_endline(
    const FileString filestring,
    const size_t current_index
) {

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
