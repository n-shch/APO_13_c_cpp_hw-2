#include "../include/utils.h"
//ну и после этой итерации он остается внутри этой дирректории. Если нужно выйти, то в конце нужно прописать это

int main(int argc, char **argv) {
//    На вход программа должна получать адрес стартовой страницы,
//    максимальную глубину прохода по ссылкам,
//    директорию, в которую необходимо положить результат.
//    адресс стартовой страницы
    char *origin_url = "https://getbootstrap.com/docs/4.4/examples/starter-template/";
//    максимальная глубина прохождения
    int count = 3;
    char *origin_dir_name = "origin_name/";
    struct list_t *web_list = list_new();
    iteration_list(origin_url, count, origin_dir_name, web_list);
    list_print(web_list);
    list_free(web_list);
    return 0;
}

