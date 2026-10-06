#include "vector/landing_spot.h"
#include <stdlib.h>

Status allocate_landing_spot_vector(LandingSpotVector *vector, size_t size_requested) {
    LandingSpot *temp = (LandingSpot*) malloc(sizeof(LandingSpot) * size_requested);
    if (temp == NULL) {return ALLOCATION_ERROR;}

    vector->capacity = size_requested;
    vector->size = 0;
    vector->array = temp;

    return NO_ERROR;
}

Status reallocate_landing_spot_vector(LandingSpotVector *vector, size_t size_requested) {
    LandingSpot *temp = (LandingSpot*) realloc(vector->array,sizeof(LandingSpot) * size_requested);
    if (temp == NULL) {return ALLOCATION_ERROR;}

    vector->capacity = size_requested;
    vector->array = temp;

    return NO_ERROR;
}

Status insert_landing_spot_vector(LandingSpotVector *vector, LandingSpot item) {
    if (vector->size + 1 >=vector->capacity) {
        Status status = reallocate_landing_spot_vector(vector, vector->capacity*2);
        if (status != NO_ERROR) {return status;}
    }

    vector->array[vector->size] = item;
    vector->size++;

    return NO_ERROR;
}

Status free_landing_spot_vector(LandingSpotVector *vector) {
    if (vector == NULL || vector->array == NULL) {return NULL_POINTER;}

    free(vector->array);
    vector->array = NULL;
    vector->size = 0;
    vector->capacity = 0;

    return NO_ERROR;
}
