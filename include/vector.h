//
// Created by nick on 11.03.2020.
//

#ifndef UNTITLED2_VECTOR_H
#define UNTITLED2_VECTOR_H


struct vec_t {
    char **url;
    int size;
};

struct vec_t *vec_new();

void vec_push(struct vec_t *vec, char *url);

void vec_free(struct vec_t *vec);
void vec_printf(struct vec_t *vec);
int vec_size(struct vec_t *vec);
char *vec_pop(struct vec_t *vec, int index);

#endif //UNTITLED2_VECTOR_H
