#include "keyword_data.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void free_keyword_data(Keyword_Data *data) {
    free(data->keywords);
    free(data->keywords_length);
    free(data->values_pair);

    data->keywords = NULL;
    data->keywords_length = NULL;
    data->values_pair = NULL;

    data->keyword_count = 0;
    data->shortest_keyword_len = 0;
    data->longest_keyword_len = 0;
}

void print_keyword_data(const Keyword_Data data) {
    unsigned char i = 0;

    for (i = 0; i < data.keyword_count; i++) {
        printf("%.*s \n", data.keywords_length[i], data.keywords + i * data.longest_keyword_len);
    }
    printf("\n");
}

void print_keyword_data_details(const Keyword_Data data) {
    unsigned char i = 0;

    for (i = 0; i < data.keyword_count; i++) {
        printf("%.*s \n", data.keywords_length[i], data.keywords + i * data.longest_keyword_len);
    }
    printf("\n");

    for (i = 0; i < data.keyword_count; i++) {
        printf("%d \n", data.keywords_length[i]);
    }
    printf("\n");

    for (i = 0; i < data.keyword_count; i++) {
        printf("%d \n", data.values_pair[i]);
    }
    printf("\n");

}

void print_partial_keyword_data_details(
    const Partial_Keyword_Data data,
    const unsigned char count,
    const unsigned char longest_keyword_len
) {
    unsigned char i = 0;

    for (i = 0; i < count; i++) {
        printf("%.*s \n", data.keywords_length[i], data.keywords + i * longest_keyword_len);
    }
    printf("\n");

    for (i = 0; i < count; i++) {
        printf("%d \n", data.keywords_length[i]);
    }
    printf("\n");

    for (i = 0; i < count; i++) {
        printf("%d \n", data.values_pair[i]);
    }
    printf("\n");

}

Keyword_Data_Status allocate_keyword_data(
    const unsigned char keyword_count,
    const unsigned char shortest_keyword_len,
    const unsigned char longest_keyword_len
) {
    Keyword_Data_Status result = {
        NO_ERROR, {NULL,NULL,NULL,0,0,0}
    };

    Keyword_Data new_data = {NULL,NULL,NULL,0,0,0};

    new_data.keyword_count = keyword_count;
    new_data.longest_keyword_len = longest_keyword_len;
    new_data.shortest_keyword_len = shortest_keyword_len;

    new_data.keywords = (char *) malloc(sizeof(char) * longest_keyword_len *keyword_count);
    if (new_data.keywords == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    new_data.keywords_length = (unsigned char *) malloc(sizeof(unsigned char) * keyword_count);
    if (new_data.keywords_length == NULL) {

        free(new_data.keywords);

        result.status = ALLOCATION_ERROR;
        return result;
    }

    new_data.values_pair = (unsigned short *) malloc(sizeof(unsigned short) * keyword_count);
    if (new_data.values_pair == NULL) {

        free(new_data.keywords);
        free(new_data.keywords_length);

        result.status = ALLOCATION_ERROR;
        return result;
    }

    result.data = new_data;
    return result;
}

Keyword_Data_Status copy_keyword_data(const Keyword_Data data) {
    Keyword_Data_Status result = {
        NO_ERROR, {NULL,NULL,NULL,0,0,0}
    };

    Keyword_Data new_data = {NULL,NULL,NULL,0,0,0};

    new_data.keyword_count = data.keyword_count;
    new_data.longest_keyword_len = data.longest_keyword_len;
    new_data.shortest_keyword_len = data.shortest_keyword_len;

    new_data.keywords = (char *) malloc(sizeof(char) * data.longest_keyword_len * data.keyword_count);
    if (new_data.keywords == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    new_data.keywords_length = (unsigned char *) malloc(sizeof(unsigned char) * data.keyword_count);
    if (new_data.keywords_length == NULL) {

        free(new_data.keywords);

        result.status = ALLOCATION_ERROR;
        return result;
    }

    new_data.values_pair = (unsigned short *) malloc(sizeof(unsigned short) * data.keyword_count);
    if (new_data.values_pair == NULL) {

        free(new_data.keywords);
        free(new_data.keywords_length);

        result.status = ALLOCATION_ERROR;
        return result;
    }

    memcpy(new_data.keywords, data.keywords, sizeof(char) * data.longest_keyword_len * data.keyword_count);
    memcpy(new_data.keywords_length, data.keywords_length, sizeof(unsigned char) * data.keyword_count);
    memcpy(new_data.values_pair, data.values_pair, sizeof(unsigned short) * data.keyword_count);


    result.data = new_data;
    return result;
}

Partial_Keyword_Data_Status copy_partial_keyword_data(
    const Keyword_Data data,
    const unsigned char row_start,
    const unsigned char row_count
) {
    Partial_Keyword_Data_Status result = {
        NO_ERROR, {NULL,NULL,NULL}
    };

    Partial_Keyword_Data new_data = {NULL,NULL,NULL};

    new_data.keywords = (char *) malloc(sizeof(char) * data.longest_keyword_len * row_count);
    if (new_data.keywords == NULL) {
        result.status = ALLOCATION_ERROR;
        return result;
    }

    new_data.keywords_length = (unsigned char *) malloc(sizeof(unsigned char) * row_count);
    if (new_data.keywords_length == NULL) {

        free(new_data.keywords);

        result.status = ALLOCATION_ERROR;
        return result;
    }

    new_data.values_pair = (unsigned short *) malloc(sizeof(unsigned short) * row_count);
    if (new_data.values_pair == NULL) {

        free(new_data.keywords);
        free(new_data.keywords_length);

        result.status = ALLOCATION_ERROR;
        return result;
    }

    memcpy(new_data.keywords, data.keywords + row_start * data.longest_keyword_len, sizeof(char) * data.longest_keyword_len * data.keyword_count);
    memcpy(new_data.keywords_length, data.keywords_length + row_start, sizeof(unsigned char) * data.keyword_count);
    memcpy(new_data.values_pair, data.values_pair + row_start, sizeof(unsigned short) * data.keyword_count);


    result.data = new_data;
    return result;
}
