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
#include "../include/vector.h"
#include "list.h"


#define ERROR_MSG -1
#define MAX_STRING_LEN 255


//проверяем, существует ли дирректория и если нет, то создаем ее
int orgin_directory(char *origin_dir_name) {
    struct stat st = {0};
    if (stat(origin_dir_name, &st) == -1) {
        mkdir(origin_dir_name, 0700);
        return 0;
    }
}


int create_folders_for_each_html_file_from_vec(struct vec_t *vec) {
    char *file_name = "folder";
    for (int i = 0; i <11; i++) {
        char index_number[10];
        sprintf(index_number, "%d", i);
        int size = strlen(file_name) + (sizeof(index_number)/sizeof(char));
        char *n = calloc(sizeof(char), size);
        n = strcpy(n, file_name);
        n = strcat(n, (char *)index_number);
        mkdir(n, 0700);
        free(n);
    }
}


char *generate_child_folder(char *par_folder, int i) {
    char *file_name = "folder";
    char index_number[10];
    sprintf(index_number, "%d/", i);
    int size = strlen(par_folder) + strlen(file_name) + (sizeof(index_number)/sizeof(char));
    char *n = calloc(sizeof(char), size);
    n = strcpy(n, par_folder);
    n = strcat(n, file_name);
    n = strcat(n, (char *)index_number);
    return n;
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




//почти потом
char *file_path_name(int i) {
    char *wget_string = "wget -O ";
    char *path_name = generate_file_name(i);
    char *file_name = "some_file.html ";

    int size = strlen(wget_string) + strlen(path_name) + strlen(file_name);
    char *n = calloc(sizeof(char), size);
    n = strcpy(n, wget_string);
    n = strcat(n, path_name);
    n = strcat(n, file_name);
    printf("%s", n);
    free(path_name);
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





//можно записать коллекцию этих адрессов в вектор и отдать его наружу
char *parse_adress(char *fd, struct vec_t *vec) {
    char *found = search_field(fd, "href=\"");
    if (found) {
        char *end = search_field(found, "\"");
        if (end) {
            char *tmp;
            asprintf(&tmp, "%.*s\n", (int)(end - found - 1), found);
            vec_push(vec, tmp);
            free(tmp);
//            printf("-----------------\n");
            return end;
        } else {
            return NULL;
        }
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
//            vec_push(vec, tmp);
            free(tmp);
            printf("-----------------\n");
            return end;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}


char *generate_url(char *url, int i) {

//    some_file - это имя любого html файла, скачанного с помощью wget
    char *wget_string = file_path_name(i);
    int size = strlen(wget_string) + strlen(url);
    char *n = calloc(sizeof(char), size);
    n = strcpy(n, wget_string);
    n = strcat(n, url);
//    system(n);
//    printf("%s",n);
    free(wget_string);
    return n;
}

//ну и после этой итерации он остается внутри этой дирректории. Если нужно выйти, то в конце нужно прописать это
int iteration(char *origin_url, int count, char *origin_dir_name) {

    orgin_directory(origin_dir_name);
//    заходим в дирректорию, теперь работаем внутри нее
    chdir(origin_dir_name);

//    генерируем запрос для wget
// складываем строку wget -O с адрессом *отуда-то*


    char *url_request = generate_origin_url(origin_url);
    system(url_request);
    free(url_request);

//    some_file.html - это name veins from generate_url

//работа с файловыми дескрипторами, открываем
    int myfd = open("some_file.html",  O_RDWR);
    if (myfd < 0) {
        perror("some_file.html open");
        exit(EXIT_FAILURE);
    };

    struct stat mystat = {};
    if (fstat(myfd, &mystat)) {
        perror("fstat error");
        exit(EXIT_FAILURE);
    }

    off_t myfsz = mystat.st_size;
    printf("docs size is: %d\n", myfsz);
    char *pointer_to_mem = mmap(NULL, myfsz, PROT_READ | PROT_WRITE, MAP_SHARED, myfd, 0);
    if (pointer_to_mem == MAP_FAILED) {
        printf("map error");
    }
    char *new_ptr;
    int ind = 0;
    struct vec_t *vec = vec_new();
    while ((new_ptr = parse_adress(pointer_to_mem,vec)) != NULL) {
        pointer_to_mem = new_ptr;
//        printf("ptr Num %d\n", ind++);
    }
//    теперь есть вектор, для которого нужно автоматизировать процесс создания отдельных дирректорий и скачивания всего остального
//    create_folders_for_each_html_file_from_vec(vec);

//    for (int i = 0; i < 10; i++) {
//        char *tmp = generate_url(vec_pop(vec,i), i);
//        system(tmp);
//        free(tmp);
//    }
//    printf("%vec_size: %d\n", vec_size(vec));
//    vec_free(vec);
    munmap(pointer_to_mem, myfsz);


    for (int i = 0; i < 3; i++) {
        return (mkdir(generate_file_name(i), 0700) && chdir("../"));
//        return (iteration(vec->url[i], count--, generate_file_name(i)) && chdir("../"));
    }
    return 1;

}



//ну и будет обычная очередю
int iteration2(int count, char *origin_dir_name) {
    orgin_directory(origin_dir_name);
//    заходим в дирректорию, теперь работаем внутри нее
    chdir(origin_dir_name);
//    получили 3 ссылки на первой итерации
    int a = 3;
//    а = колличество ссылок внутри файла для всех для них нужно будет пройтись
    if (count > 0) {
        --count;
        for (;a > 0; --a) {
            iteration2(count, generate_file_name((a)));
            chdir("../");
        }
    }
}



int iteration_list(char *origin_url, int count, char *origin_dir_name, struct list_t *list) {
    orgin_directory(origin_dir_name);
    chdir(origin_dir_name);
    char *url_request = generate_origin_url(origin_url);
    system(url_request);
    free(url_request);
    int myfd = open("some_file.html",  O_RDWR);
    if (myfd < 0) {
        perror("some_file.html open");
        exit(EXIT_FAILURE);
    };
    struct stat mystat = {};
    if (fstat(myfd, &mystat)) {
        perror("fstat error");
        exit(EXIT_FAILURE);
    }
    off_t myfsz = mystat.st_size;
    printf("docs size is: %d\n", myfsz);
    char *pointer_to_mem = mmap(NULL, myfsz, PROT_READ | PROT_WRITE, MAP_SHARED, myfd, 0);
    if (pointer_to_mem == MAP_FAILED) {
        printf("map error");
    }
    char *new_ptr;
    int ind = 0;
    while ((new_ptr = parse_adress_l(pointer_to_mem,list)) != NULL) {
        pointer_to_mem = new_ptr;
        ind++;
    }
    printf("amount of href: %d\n", ind);
    munmap(pointer_to_mem, myfsz);


    if (count > 0) {
        --count;
        for (;ind > 0; --ind) {
            struct node_t *tmp = list_pop(list);

            iteration_list(tmp->url, count, generate_file_name(ind), list);
            chdir("../");
            free(tmp->url);
            free(tmp);
        }
    }
}




int iteration2_list(int count, char *origin_dir_name, char *origin_url, struct list_t *list) {
    orgin_directory(origin_dir_name);
//    заходим в дирректорию, теперь работаем внутри нее
    chdir(origin_dir_name);
    char *url_request = generate_origin_url(origin_url);
    system(url_request);
    free(url_request);
    int myfd = open("some_file.html",  O_RDWR);
    if (myfd < 0) {
        perror("some_file.html open");
        exit(EXIT_FAILURE);
    };
    struct stat mystat = {};
    if (fstat(myfd, &mystat)) {
        perror("fstat error");
        exit(EXIT_FAILURE);
    }
    off_t myfsz = mystat.st_size;
    printf("docs size is: %d\n", myfsz);
    char *pointer_to_mem = mmap(NULL, myfsz, PROT_READ | PROT_WRITE, MAP_SHARED, myfd, 0);
    if (pointer_to_mem == MAP_FAILED) {
        printf("map error");
    }
    char *new_ptr;
    int ind = 0;
    while ((new_ptr = parse_adress_l(pointer_to_mem,list)) != NULL) {
        pointer_to_mem = new_ptr;
        ind++;
    }
    close(myfd);
    munmap(pointer_to_mem, myfsz);
//    получили 3 ссылки на первой итерации
//    а = колличество ссылок внутри файла для всех для них нужно будет пройтись
    if (count > 0) {
        --count;
        for (;ind > 0; --ind) {
            iteration2_list(count, generate_file_name((ind)), origin_url, list);
            chdir("../");
        }
    }
}
