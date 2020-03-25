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
#include "testy/list.h"

#define ERROR_MSG -1
#define MAX_DIGIT_IN_INDEX 10


//  проверяем, существует ли дирректория и если нет, то создаем ее
int orgin_directory(char *origin_dir_name) {
    struct stat st = {0};
    if (stat(origin_dir_name, &st) == -1) {
        mkdir(origin_dir_name, 0700);
        return 0;
    }
}


//  генерируем имя директории типа folderN/, где N - порядковый номер ссылки в родительском файле
char *generate_file_name(int i) {
    char *file_name = "folder";
    char index_number[MAX_DIGIT_IN_INDEX];
    sprintf(index_number, "%d/", i);
    int size = strlen(file_name) + (sizeof(index_number)/sizeof(char));
    char *n = calloc(sizeof(char), size);
    if (!n) {
        return NULL;
    }
    n = strcpy(n, file_name);
    n = strcat(n, (char *)index_number);
    return n;
}

//  генерация wget запроса по заданному аргументу-урлу
//  имя файла, куда выкачиваем страницу всегда some_file.html для удобства дальнейшего парсинга
char *generate_origin_url(char *url) {
    char *wget_string = "wget -O some_file.html ";\
    int size = strlen(wget_string) + strlen(url);
    char *n = calloc(sizeof(char), size);
    if (!n) {
        perror("calloc failed\n");
        return NULL;
    }
    n = strcpy(n, wget_string);
    n = strcat(n, url);
    return n;
}

//  поиск подстроки в строке, возвращение указателя на начало подстроки с ИНФОРМАЦИЕЙ
char *search_field(char *mem_space, char *field) {
    char *found = strstr(mem_space, field);
    if (found) {
        found += (strlen(field));
        return found;
    } else {
        return NULL;
    }
}

//  ищем поля формата href="..."
//  забираем содержимое внутри кавычек, добавляем новый элемент в лист
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

// итерация, которая происходит на последнем уровне глубины.
// Отличается от iteration_list тем, что нам не интересны ссылки внутри скачанных файлов
// поэтому парсинга не происходит
int iteration_list_part(char *origin_url, int count, char *origin_dir_name, struct list_t *list) {
    orgin_directory(origin_dir_name);
    chdir(origin_dir_name);
    char *url_request = generate_origin_url(origin_url);
    if (url_request) {
        system(url_request);
        free(url_request);
    }
}

//  единица итерации, скачиваем файл, заданный ссылкой, в заданную папку.
//  пробегаемся по файлу, все внутрилежащие ссылки записываем в list
int iteration_list(char *origin_url, int count, char *origin_dir_name, struct list_t *list) {
    orgin_directory(origin_dir_name);
    chdir(origin_dir_name);
    char *url_request = generate_origin_url(origin_url);
    if (!url_request) {
        return ERROR_MSG;
    }
//  отправка wget запроса в bash
    system(url_request);
    free(url_request);
//  начинаем парсинг
    int myfd = open("some_file.html",  O_RDWR);
    if (myfd < 0) {
        printf("some_file.html open error");
        return ERROR_MSG;
    }
    struct stat mystat = {};
    if (fstat(myfd, &mystat)) {
        printf("fstat error");
        return ERROR_MSG;
    }

    off_t myfsz = mystat.st_size;
    printf("docs size is: %ld\n", myfsz);
    char *pointer_to_mem = mmap(NULL, myfsz, PROT_READ | PROT_WRITE, MAP_SHARED, myfd, 0);
    if (pointer_to_mem == MAP_FAILED) {
        close(myfd);
        printf("map error\n");
        return ERROR_MSG;
    }
    char *new_ptr;
    int ind = 0;
    while ((new_ptr = parse_adress_l(pointer_to_mem, list)) != NULL) {
        pointer_to_mem = new_ptr;
        ind++;
    }
    printf("amount of href: %d\n", ind);
    munmap(pointer_to_mem, myfsz);
    close(myfd);

//  Дальше идет рекурсивный вызов этой же функции
//  проверяем уровень глубины, если он не 0, то работаем дальше
    if (count > 0) {
//  если уровень глубины не последний, тогда нам интересны ссылки, находящиеся в файлах
//  применяем такую же итерацию: скачивание + парсинг файла
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
//  иначе, если это последний уровень, нам более, чем достаточно просто выкачать файл и положить его в директорию
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





