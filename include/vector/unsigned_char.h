#ifndef UNSIGNED_CHAR_H
#define UNSIGNED_CHAR_H

#include "status.h"
#include <stddef.h>
#include <stdint.h>

typedef struct unsigned_char_Vector {
    unsigned char *data;
    size_t size;
    size_t capacity;
} unsigned_char_Vector;

typedef struct unsigned_char_Vector_Status {
    Status status;
    unsigned_char_Vector vector;
}unsigned_char_Vector_Status;

unsigned_char_Vector_Status allocate_unsigned_char_vector(const size_t size_requested);
Status reallocate_unsigned_char_vector(unsigned_char_Vector *vector, const size_t size_requested);

Status insert_unsigned_char_vector(unsigned_char_Vector *vector, const unsigned char item);
void free_unsigned_char_vector(unsigned_char_Vector *vector);

unsigned_char_Vector_Status copy_unsigned_char_vector(const unsigned_char_Vector vector);



#endif
