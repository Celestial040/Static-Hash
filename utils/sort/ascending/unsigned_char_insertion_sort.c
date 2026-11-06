#include "sort/ascending/unsigned_char_insertion_sort.h"
#include "vector/unsigned_char.h"

Status insertion_sort(unsigned_char_Vector *vector) {
    size_t i = 1;
    size_t j = 0;
    unsigned char temp = 0;

    while (i < vector->size) {
        temp = vector->data[i];
        j = i;
        while (j > 0 && vector->data[j-1] > temp) {
            vector->data[j] = vector->data[j-1];
            j--;
        }
        vector->data[j] = temp;
        i++;
    }

    return NO_ERROR;
}

Status last_item_insertion_sort(unsigned_char_Vector *vector) {
    size_t i = vector->size - 1;
    size_t j = 0;
    unsigned char temp = vector->data[i];

    temp = vector->data[i];
    j = i;
    while (j > 0 && vector->data[j-1] > temp) {
        vector->data[j] = vector->data[j-1];
        j--;
    }
    vector->data[j] = temp;
    i++;

    return NO_ERROR;
}
