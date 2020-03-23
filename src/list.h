//
// Created by nick on 23.03.2020.
//

#ifndef CRAWLER_LIST_H
#define CRAWLER_LIST_H


struct node_t {
    int index;
    char *url;
    struct node_t *next;
    struct node_t *previous;
};


struct list_t {
    struct node_t *head;
    struct node_t *tail;
    int size;
};

#endif //CRAWLER_LIST_H

int list_size(struct list_t *list);
struct list_t *list_new();
void list_push(struct list_t *list, const char *url);

struct node_t *list_pop(struct list_t *list);
void list_print(struct list_t *list);