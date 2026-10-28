#ifndef BEST_COLUMNS_VECTOR_H
#define BEST_COLUMNS_VECTOR_H

#include "../status.h"
#include <stddef.h>
#include <stdint.h>


typedef unsigned char BestColumns;

typedef struct BestColumnsVector {
    BestColumns *array;
    size_t size;
    size_t capacity;
} BestColumnsVector;

Status allocate_best_columns_vector(BestColumnsVector *vector, size_t size_requested);
Status reallocate_best_columns_vector(BestColumnsVector *vector, size_t size_requested);
Status insert_best_columns_vector(BestColumnsVector *vector, BestColumns item);
Status free_best_columns_vector(BestColumnsVector *vector);


#endif
