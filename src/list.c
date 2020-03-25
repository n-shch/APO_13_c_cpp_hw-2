//
// Created by nick on 11.03.2020.
//
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include "testy/list.h"

int list_size(struct list_t *list) {
    return list->size;
}

struct list_t *list_new() {
    return calloc(sizeof(struct list_t), 1);
}

void list_push(struct list_t *list, const char *url) {
    if (!list || !url) {
        return;
    } else {
        struct node_t *tmp = calloc(sizeof(struct node_t), 1);
        asprintf(&tmp->url, "%s", url);
        if (!list->head && !list->tail) {
            list->head = tmp;
            list->tail = tmp;
            list->size++;
        } else {
            tmp->previous = list->tail;
            list->tail->next = tmp;
            list->tail = tmp;
            list->size++;
        }
    }
}

struct node_t *list_pop(struct list_t *list) {
    struct node_t *tmp = list->tail;
    if (list->tail == list->head) {
        list->size--;
        return list->head;
    } else {
        list->tail = list->tail->previous;
        list->tail->next = NULL;
        list->size--;
        return tmp;
    }
}

void list_print(struct list_t *list) {
    if (!list) {
        printf("no such an entity to print\n");
        return;
    }
    printf("size of list = %d\n",list->size);
    for (int i = list_size(list); i > 0; i--) {
        struct node_t *tmp = list_pop(list);
        printf("%d: %s\n", list->size, tmp->url);
        if (tmp->url) {
            free(tmp->url);
        }
        free(tmp);
    }
    free(list);
}

