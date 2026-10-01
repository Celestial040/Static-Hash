#ifndef STATUS_H
#define STATUS_H

typedef unsigned char Status;

enum {
    NO_ERROR,
    ALLOCATION_ERROR,
    FILE_NOT_FOUND,
    NULL_POINTER,
    STRING_EMPTY,
    FIELD_MATCH_NOT_FOUND,
    ITEM_NOT_FOUND,
    ITEM_FOUND,
    READ_ERROR,
    MISMATCH_EXPECTATION,
    DIGITS_TOO_LONG,
    INTEGER_OVERFLOW,
    INDEX_OUT_OF_BOUND
} ;


void status_print(Status status);

#endif
