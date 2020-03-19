//
// Created by nick on 11.03.2020.
//


#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include "../include/vector.h"



struct vec_t *vec_new() {
    return calloc(sizeof(struct vec_t), 1);
}

void vec_push(struct vec_t *vec, char *url) {
    if (!vec || !url) {
        return;
    } else {
        vec->url = realloc(vec->url, ++vec->size * sizeof(url));
        vec->url[vec->size -1] = calloc(sizeof(char *), 1);
        asprintf(&vec->url[vec->size - 1], "%s", url);
    }
}

void vec_free(struct vec_t *vec) {
    for (int i = 0; i < vec->size; i++) {
        free(vec->url[i]);
    }
    free(vec->url);
    free(vec);
}
void vec_printf(struct vec_t *vec) {
    for (int i = 0; i < vec->size; i++) {
        printf("%d: %s\n", i, vec->url[i]);
    }
}

int vec_size(struct vec_t *vec) {
    return vec->size;
}

char *vec_pop(struct vec_t *vec, int index) {
    return vec->url[index];
}