//
// Created by nick on 09.03.2020.
//
#ifndef CRAWLER_UTILS_H
#define CRAWLER_UTILS_H

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


int orgin_directory(char *origin_dir_name);
char *generate_file_name(int i);
char *generate_origin_url(char *url);
char *search_field(char *mem_space, char *field);
char *parse_adress_l(char *fd, struct list_t *list);
int iteration_list_part(char *origin_url, int count, char *origin_dir_name, struct list_t *list);
int iteration_list(char *origin_url, int count, char *origin_dir_name, struct list_t *list);



#endif // CRAWLER_UTILS_H

