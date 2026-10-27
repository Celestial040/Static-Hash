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

TokenPointer lexer_scan(const FileString file_string, const size_t index_position){
    TypeMode prev_mode = TAB_SPACE;
    TypeMode current_mode = TAB_SPACE;
    size_t head = index_position;
    size_t tail = index_position;
    TokenPointer tokenpointer = {0,0,TOKEN_UNKNOWN};

    while (head < file_string.length) {
        current_mode = type_check(file_string.start[head]);

        if (prev_mode == NUMERIC_LITERAL && is_numeric(file_string.start[head])) { current_mode = NUMERIC_LITERAL;}

        if (current_mode != prev_mode || (current_mode == SYMBOL && prev_mode == SYMBOL)) {

            if (current_mode == ALPHANUMERIC && is_numeric(file_string.start[head])) {
                current_mode = NUMERIC_LITERAL;
            }

            switch (prev_mode) {
                case TAB_SPACE:
                    tail = head;
                    prev_mode = current_mode;
                    head++;
                    continue;

                case ALPHANUMERIC:
                    tokenpointer.token = TOKEN_IDENTIFIER;
                    tokenpointer.start = tail;
                    tokenpointer.len = head - tail;

                    tail = head;
                    prev_mode = current_mode;
                    head++;

                    return tokenpointer;

                case SYMBOL:
                    tokenpointer.token = token_symbol_enum_check(file_string.start[tail]);
                    tokenpointer.start = tail;
                    tokenpointer.len = head - tail;

                    tail = head;
                    prev_mode = current_mode;
                    head++;

                    return tokenpointer;

                case NUMERIC_LITERAL:
                    tokenpointer.token = TOKEN_NUMERIC_LITERAL;
                    tokenpointer.start = tail;
                    tokenpointer.len = head - tail;

                    tail = head;
                    prev_mode = current_mode;
                    head++;

                    return tokenpointer;

                case ENDLINE:
                    tokenpointer.token = TOKEN_ENDLINE;
                    tokenpointer.start = tail;
                    tokenpointer.len = head - tail;

                    tail = head;
                    prev_mode = current_mode;
                    head++;

                    return tokenpointer;

                default:
                    break;
            }
        }
        head++;
    }

    if (head >= file_string.length) {
        tokenpointer.token = TOKEN_EOF;
        tokenpointer.start = file_string.length;
        tokenpointer.len = 0;
    };


    return tokenpointer;
}
