#include "vector/unsigned_char.h"
#include "status.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned_char_Vector_Status allocate_unsigned_char_vector(const size_t size_requested) {
    unsigned_char_Vector_Status result = {NO_ERROR, {NULL, 0, 0}};

    unsigned_char_Vector new_vector = {NULL, 0, 0};

    new_vector.capacity = size_requested;
    new_vector.size = 0;

    new_vector.data = (unsigned char *) malloc(sizeof(unsigned char) * size_requested);

    if (new_vector.data == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    result.vector = new_vector;
    return result;
}

Status reallocate_unsigned_char_vector(unsigned_char_Vector *vector, const size_t size_requested) {
    unsigned char *temp = (unsigned char*) realloc(
        vector->data,
        sizeof(unsigned char) * size_requested
    );

    if (temp == NULL) {return ALLOCATION_ERROR;}

    vector->capacity = size_requested;
    vector->data = temp;

    return NO_ERROR;
}

unsigned_char_Vector_Status copy_unsigned_char_vector(const unsigned_char_Vector vector) {
    unsigned_char_Vector_Status result = {NO_ERROR, {NULL, 0, 0}};

    unsigned_char_Vector_Status vector_copy = {NO_ERROR,{NULL, 0, 0}};

    vector_copy = allocate_unsigned_char_vector(vector.capacity);
    if (vector_copy.status != NO_ERROR) {
        result.status = vector_copy.status;
        return result;
    }

    vector_copy.vector.size = vector.size;
    vector_copy.vector.capacity = vector.capacity;

    memcpy(vector_copy.vector.data, vector.data, sizeof(unsigned char) * vector.size);

    result.vector = vector_copy.vector;
    return result;
}


Status insert_unsigned_char_vector(unsigned_char_Vector *vector, const unsigned char item) {
    if (vector->size + 1 >=vector->capacity) {
        Status status = reallocate_unsigned_char_vector(
            vector,
            vector->capacity*2
        );

        if (status != NO_ERROR) {return status;}
    }

    vector->data[vector->size] = item;
    vector->size++;

    return NO_ERROR;
}

void free_unsigned_char_vector(unsigned_char_Vector *vector) {
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}
