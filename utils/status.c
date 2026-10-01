
#include "status.h"
#include <stdio.h>

void status_print(Status status) {
    switch (status) {
        case NO_ERROR:
            printf("NO_ERROR\n");
            break;
        case ALLOCATION_ERROR:
            printf("ALLOCATION_ERROR\n");
            break;
        case FILE_NOT_FOUND:
            printf("FILE_NOT_FOUND\n");
            break;
        case NULL_POINTER:
            printf("NULL_POINTER\n");
            break;
        case FIELD_MATCH_NOT_FOUND:
            printf("FIELD_MATCH_NOT_FOUND\n");
            break;
        case ITEM_NOT_FOUND:
            printf("ITEM_NOT_FOUND\n");
            break;
        case ITEM_FOUND:
            printf("ITEM_FOUND\n");
            break;
        case STRING_EMPTY:
            printf("STRING_EMPTY\n");
            break;
        case READ_ERROR:
            printf("READ_ERROR\n");
            break;
        case MISMATCH_EXPECTATION:
            printf("MISMATCH_EXPECTATION\n");
            break;
        case DIGITS_TOO_LONG:
            printf("DIGITS_TOO_LONG\n");
            break;
        case INTEGER_OVERFLOW:
            printf("INTEGER_OVERFLOW\n");
            break;
        case INDEX_OUT_OF_BOUND:
            printf("INTEGER_OVERFLOW\n");
            break;
        default:
            printf("UNKNOWN_STATUS\n");
            break;
    }

}
