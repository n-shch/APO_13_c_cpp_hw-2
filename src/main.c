#include "testy/utils.h"

   /*На вход программа должна получать адрес стартовой страницы,
    максимальную глубину прохода по ссылкам,
    директорию, в которую необходимо положить результат.
    адресс стартовой страницы
    для удобства тестирования захардкодим эти данные, но при желании можно
    передавать их через аргументы функции main
    */

int main(int argc, char **argv) {
//адрес стартовой страницы
    char *origin_url = "https://getbootstrap.com/docs/4.4/examples/starter-template/";
//  максимальная глубина прохода по ссылкам
    int count = 3;
//  имя директории, в котором будут хранится загруженные данные
    char *origin_dir_name = "origin_name/";

    struct list_t *web_list = list_new();
    if (!web_list) {
        perror("calloc failed\n");
        return ERROR_MSG;
    }
    iteration_list(origin_url, count, origin_dir_name, web_list);
    list_print(web_list);
    return 0;
}

