//
// Created by nick on 09.03.2020.
//

#ifndef INC_1_SEM_C_CPP_IZ_2_UTILS_H
#define INC_1_SEM_C_CPP_IZ_2_UTILS_H

#define ERROR_MSG -1
#define MAX_STRING_LEN 255


//проверяем, существует ли дирректория и если нет, то создаем ее
int orgin_directory(char *origin_dir_name);
int create_folders_for_each_html_file_from_vec(struct vec_t *vec);
//выводит указатель на строку folder_i, нужно чистить
char *generate_file_name(int i);
char *generate_child_folder(char *par_folder, int i);
//почти потом
char *file_path_name(int i);
char *generate_origin_url(char *url);
char *search_field(char *mem_space, char *field);
//можно записать коллекцию этих адрессов в вектор и отдать его наружу
char *parse_adress(char *fd, struct vec_t *vec);
char *generate_url(char *url, int i);
int iteration(char *origin_url, int count, char *origin_dir_name, struct vec_t *vec);
int iteration2(int count, char *origin_dir_name);
int iteration_list(char *origin_url, int count, char *origin_dir_name, struct list_t *list);
int iteration2_list(int count, char *origin_dir_name, char *origin_url, struct list_t *list);
#endif //INC_1_SEM_C_CPP_IZ_2_UTILS_H
