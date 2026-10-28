#include "vector/best_columns.h"
#include <stdlib.h>

Status allocate_best_columns_vector(
    BestColumnsVector *vector,
    size_t size_requested
) {
    BestColumns *temp = (BestColumns*) malloc(
        sizeof(BestColumns) * size_requested
    );

    if (temp == NULL) {return ALLOCATION_ERROR;}

    vector->capacity = size_requested;
    vector->size = 0;
    vector->array = temp;

    return NO_ERROR;
}

Status reallocate_best_columns_vector(
    BestColumnsVector *vector,
    size_t size_requested
) {

    BestColumns *temp = (BestColumns*) realloc(
        vector->array,
        sizeof(BestColumns) * size_requested
    );

    if (temp == NULL) {return ALLOCATION_ERROR;}

    vector->capacity = size_requested;
    vector->array = temp;

    return NO_ERROR;
}

Status insert_best_columns_vector(
    BestColumnsVector *vector,
    BestColumns item
) {
    if (vector->size + 1 >=vector->capacity) {
        Status status = reallocate_best_columns_vector(
            vector,
            vector->capacity*2
        );

        if (status != NO_ERROR) {return status;}
    }

    vector->array[vector->size] = item;
    vector->size++;

    return NO_ERROR;
}

Status free_best_columns_vector(BestColumnsVector *vector) {
    if (vector == NULL || vector->array == NULL) {
        return NULL_POINTER;
    }

    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;

    return NO_ERROR;
}
