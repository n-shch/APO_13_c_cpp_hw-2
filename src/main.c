#define _GNU_SOURCE         /* See feature_test_macros(7) */

#include "../include/utils.h"




int main() {
//    На вход программа должна получать адрес стартовой страницы,
//    максимальную глубину прохода по ссылкам,
//    директорию, в которую необходимо положить результат.
//    адресс стартовой страницы
    char *origin_url = "https://cboard.cprogramming.com/c-programming/135789-wget-c.html";
//    максимальная глубина прохождения
    int count = 5;

//    имя директории, куда положить файл
    char *origin_dir_name = "origin_name";
    iteration(origin_url,count,origin_dir_name);
    return 0;
}

