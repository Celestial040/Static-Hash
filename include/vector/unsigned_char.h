#ifndef UNSIGNED_CHAR_H
#define UNSIGNED_CHAR_H

#include "status.h"
#include <stddef.h>
#include <stdint.h>

typedef struct Unsigned_Char_Vector {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Unsigned_Char_Vector;

typedef struct Unsigned_Char_Vector_Status {
    Status status;
    Unsigned_Char_Vector vector;
}Unsigned_Char_Vector_Status;

Unsigned_Char_Vector_Status allocate_unsigned_char_vector(const size_t size_requested);
Status reallocate_unsigned_char_vector(Unsigned_Char_Vector *vector, const size_t size_requested);

Status insert_unsigned_char_vector(Unsigned_Char_Vector *vector, const unsigned char item);
void free_unsigned_char_vector(Unsigned_Char_Vector *vector);

Unsigned_Char_Vector_Status copy_unsigned_char_vector(const Unsigned_Char_Vector vector);



#endif
