#define _GNU_SOURCE         /* See feature_test_macros(7) */

#include <zconf.h>
#include <sys/stat.h>
#include "../include/utils.h"
#include "../include/vector.h"
#include "stdio.h"
#include "list.h"

//ну и после этой итерации он остается внутри этой дирректории. Если нужно выйти, то в конце нужно прописать это

int main() {
//    На вход программа должна получать адрес стартовой страницы,
//    максимальную глубину прохода по ссылкам,
//    директорию, в которую необходимо положить результат.
//    адресс стартовой страницы
    char *origin_url = "https://cboard.cprogramming.com/c-programming/135789-wget-c.html";
//    максимальная глубина прохождения
    int count = 1;

//    имя директории, куда положить файл
//    struct vec_t *vec = vec_new();

    char *origin_dir_name = "origin_name/";
    struct list_t *web_list = list_new();
    iteration_list(origin_url, count, origin_dir_name, web_list);
    list_print(web_list);
    return 0;
}

