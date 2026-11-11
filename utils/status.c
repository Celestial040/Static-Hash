
#include "status.h"
#include <stdio.h>

void status_print(Status status) {

    const char *error_strings[17] = {
        "NO_ERROR",
        "ALLOCATION_ERROR",
        "FILE_NOT_FOUND",
        "NULL_POINTER",
        "STRING_EMPTY",
        "FIELD_MATCH_NOT_FOUND",
        "ITEM_NOT_FOUND",
        "ITEM_FOUND",
        "READ_ERROR",
        "MISMATCH_EXPECTATION",
        "INTEGER_OVERFLOW",
        "INDEX_OUT_OF_BOUND",
        "STRING_TOO_LONG",
        "DIGITS_TOO_LONG",
        "MAX_LIMIT_EXCEEDED",
        "END_OF_FILE",
        "KEYWORD_DUPLICATE_ERROR"
    };

    if (status > 16) {
        printf("UNKNOWN_ERROR");
        return;
    }

    printf("%s \n", error_strings[status]);
    return;
}
