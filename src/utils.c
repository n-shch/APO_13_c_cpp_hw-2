//
// Created by nick on 07.03.2020.
//
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "string.h"
#include "../include/list.h"

#define ERROR_MSG -1


//проверяем, существует ли дирректория и если нет, то создаем ее
int orgin_directory(char *origin_dir_name) {
    struct stat st = {0};
    if (stat(origin_dir_name, &st) == -1) {
        mkdir(origin_dir_name, 0700);
        return 0;
    }
}


//выводит указатель на строку folder_i, нужно чистить
char *generate_file_name(int i) {
    char *file_name = "folder";
    char index_number[10];
    sprintf(index_number, "%d/", i);
    int size = strlen(file_name) + (sizeof(index_number)/sizeof(char));
    char *n = calloc(sizeof(char), size);
    n = strcpy(n, file_name);
    n = strcat(n, (char *)index_number);
    return n;
}







char *generate_origin_url(char *url) {
//    some_file - это имя любого html файла, скачанного с помощью wget
    char *wget_string = "wget -O some_file.html ";\
    int size = strlen(wget_string) + strlen(url);
    char *n = calloc(sizeof(char), size);
    n = strcpy(n, wget_string);
    n = strcat(n, url);
//    system(n);
//    printf("%s",n);
//    free(wget_string);
    return n;
}


char *search_field(char *mem_space, char *field) {
    char *found = strstr(mem_space, field);
    if (found) {
        found += (strlen(field));
        return found;
    } else {
        return NULL;
    }
}




char *parse_adress_l(char *fd, struct list_t *list) {
    char *found = search_field(fd, "href=\"");
    if (found) {
        char *end = search_field(found, "\"");
        if (end) {
            char *tmp;
            asprintf(&tmp, "%.*s\n", (int)(end - found - 1), found);
            list_push(list,tmp);
            free(tmp);
            return end;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

int iteration_list_part(char *origin_url, int count, char *origin_dir_name, struct list_t *list) {
    orgin_directory(origin_dir_name);
    chdir(origin_dir_name);
    char *url_request = generate_origin_url(origin_url);
    if (url_request) {
        system(url_request);
        if (url_request) {
            free(url_request);
        }
    }
}




int iteration_list(char *origin_url, int count, char *origin_dir_name, struct list_t *list) {
    orgin_directory(origin_dir_name);
    chdir(origin_dir_name);
    char *url_request = generate_origin_url(origin_url);
    if (url_request) {
        system(url_request);
        if (url_request) {
            free(url_request);
        }
        int myfd = open("some_file.html",  O_RDWR);
        if (myfd < 0) {
            printf("some_file.html open");
            return -1;
        } else {
            struct stat mystat = {};
            if (fstat(myfd, &mystat)) {
                printf("fstat error");
                return -1;
            } else {
                off_t myfsz = mystat.st_size;
                printf("docs size is: %d\n", myfsz);
                char *pointer_to_mem = mmap(NULL, myfsz, PROT_READ | PROT_WRITE, MAP_SHARED, myfd, 0);
                if (pointer_to_mem == MAP_FAILED) {
                    close(myfd);
                    printf("map error\n");
                    return -1;
                } else {
                    char *new_ptr;
                    int ind = 0;
                    while ((new_ptr = parse_adress_l(pointer_to_mem,list)) != NULL) {
                        pointer_to_mem = new_ptr;
                        ind++;
                    }
                    printf("amount of href: %d\n", ind);
                    munmap(pointer_to_mem, myfsz);
                    close(myfd);
                    if (count > 0) {
                        if (count > 1) {
                            --count;
                            for (;ind > 0; --ind) {
                                struct node_t *tmp = list_pop(list);
                                if (tmp) {
                                    char *tmp_file_name = generate_file_name(ind);
                                    iteration_list(tmp->url, count, tmp_file_name, list);
                                    free(tmp_file_name);
                                    chdir("../");
                                    if (tmp->url) {
                                        free(tmp->url);
                                    }
                                    free(tmp);
                                }

                            }
                        } else {
                            for (;ind > 0; --ind) {
                                struct node_t *tmp = list_pop(list);
                                char *tmp_file_name = generate_file_name(ind);
                                iteration_list_part(tmp->url, count, tmp_file_name, list);
                                free(tmp_file_name);
                                chdir("../");
                                if (tmp->url) {
                                    free(tmp->url);
                                }
                                free(tmp);
                            }
                        }
                    }
                }
            }
        }
    }
}





