#ifndef LANDING_SPOT_VECTOR_H
#define LANDING_SPOT_VECTOR_H

#include "../status.h"
#include <stddef.h>
#include <stdint.h>


typedef unsigned char LandingSpot;

typedef struct LandingSpotVector {
    LandingSpot *array;
    size_t size;
    size_t capacity;
} LandingSpotVector;

Status allocate_landing_spot_vector(LandingSpotVector *vector, size_t size_requested);
Status reallocate_landing_spot_vector(LandingSpotVector *vector, size_t size_requested);
Status insert_landing_spot_vector(LandingSpotVector *vector, LandingSpot item);
Status free_landing_spot_vector(LandingSpotVector *vector);


#endif
