#include "lexer.h"
#include "char_manip.h"
#include <stdlib.h>

TypeMode type_check(const char target) {
    if (is_alphabet_numeric(target)) {
        return ALPHANUMERIC;
    } else if (is_tab_space(target)) {
        return TAB_SPACE;
    } else if (is_endline(target)) {
        return ENDLINE;
    } else {
        return SYMBOL;
    }
}

Token token_symbol_enum_check(const char target) {
    switch (target) {
        case '=':
            return TOKEN_ASSIGNMENT;

        case '(':
            return TOKEN_OPEN_BRACKET;

        case ')':
            return TOKEN_CLOSE_BRACKET;

        case '{':
            return TOKEN_OPEN_CURLY_BRACKET;

        case '}':
            return TOKEN_CLOSE_CURLY_BRACKET;

        case ',':
            return TOKEN_COMMA;

        default:
            return TOKEN_UNKNOWN;
    }
}

static TypeMode prev_mode, current_mode = TAB_SPACE;
static size_t index, trail = 0;
static TokenPointer tokenpointer;


void set_lexer_index(size_t checkpoint) {
    index = trail = checkpoint;
    prev_mode = current_mode = TAB_SPACE;
}

size_t get_lexer_tail(void) {
    return trail;
}

TokenPointer lexer_scan(const FileString *file_string) {
    tokenpointer.token = TOKEN_UNKNOWN;
    tokenpointer.start = 0;
    tokenpointer.len = 0;

    while (index < file_string->length) {
        current_mode = type_check(file_string->start[index]);

        if (prev_mode == NUMERIC_LITERAL && is_numeric(file_string->start[index])) { current_mode = NUMERIC_LITERAL;}

        if (current_mode != prev_mode || (current_mode == SYMBOL && prev_mode == SYMBOL)) {

            if (current_mode == ALPHANUMERIC && is_numeric(file_string->start[index])) {
                current_mode = NUMERIC_LITERAL;
            }

            switch (prev_mode) {
                case TAB_SPACE:
                    trail = index;
                    prev_mode = current_mode;
                    index++;
                    continue;

                case ALPHANUMERIC:
                    tokenpointer.token = TOKEN_IDENTIFIER;
                    tokenpointer.start = trail;
                    tokenpointer.len = index - trail;

                    trail = index;
                    prev_mode = current_mode;
                    index++;

                    return tokenpointer;

                case SYMBOL:
                    tokenpointer.token = token_symbol_enum_check(file_string->start[trail]);
                    tokenpointer.start = trail;
                    tokenpointer.len = index - trail;

                    trail = index;
                    prev_mode = current_mode;
                    index++;

                    return tokenpointer;

                case NUMERIC_LITERAL:
                    tokenpointer.token = TOKEN_NUMERIC_LITERAL;
                    tokenpointer.start = trail;
                    tokenpointer.len = index - trail;

                    trail = index;
                    prev_mode = current_mode;
                    index++;

                    return tokenpointer;

                case ENDLINE:
                    tokenpointer.token = TOKEN_ENDLINE;
                    tokenpointer.start = trail;
                    tokenpointer.len = index - trail;

                    trail = index;
                    prev_mode = current_mode;
                    index++;

                    return tokenpointer;

                default:
                    break;
            }
        }
        index++;
    }

    if (index >= file_string->length) {tokenpointer.token = TOKEN_EOF;}

    return tokenpointer;
}
