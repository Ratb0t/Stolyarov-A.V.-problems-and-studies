#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>

#include "my_list.h"
#include "my_string.h"

int str_cmp(const char *str1, const char *str2)
{
    for (int i = 0; str1[i] || str2[i]; ++i)
    {
        if ((str1[i] | 0x20) != (str2[i] | 0x20))
        {
            return 0;
        }
    }
    return 1;
}

void search_file_iterative(my_string *start_path, my_string *target)
{
    my_list *stack = my_list_create();
    if (stack == NULL)
    {
        fprintf(stderr, "Ошибка создания списка\n");
        return;
    }

    // Создаем копию начального пути для стека
    my_string *start_path_copy = my_str_create(my_str_get_data(start_path));
    my_list_push_front(stack, (top_type){.as_void = start_path_copy});

    while (my_list_get_len(stack) > 0)
    {
        // Извлекаем путь из стека
        my_string *current_path = my_list_delete_item(stack, 0).as_void;
        if (current_path == NULL)
        {
            continue;
        }

        const char *path_str = my_str_get_data(current_path);
        if (path_str == NULL)
        {
            my_str_destroy(current_path);
            continue;
        }

        DIR *dir = opendir(path_str);
        if (dir == NULL)
        {
            perror("opendir");
            my_str_destroy(current_path);
            continue;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            // Пропускаем текущую и родительскую директории
            if (str_cmp(entry->d_name, ".") || str_cmp(entry->d_name, ".."))
            {
                continue;
            }

            // Формируем полный путь: current_path + "/" + entry->d_name
            my_string *tmp2;
            
            my_string *tmp1 = my_str_create("/");
            my_string *fullpath = my_str_create(path_str);
            tmp2 = my_str_concatenation(fullpath, tmp1);
            my_str_destroy(fullpath);
            my_str_destroy(tmp1);

            tmp1 = my_str_create(entry->d_name);
            fullpath = my_str_concatenation(tmp2, tmp1);
            my_str_destroy(tmp1);
            my_str_destroy(tmp2);

            struct stat statbuf;
            if (stat(my_str_get_data(fullpath), &statbuf) == -1)
            {
                perror("stat");
                my_str_destroy(fullpath);
                continue;
            }

            // Если это директория - добавляем в стек для последующего обхода
            if (S_ISDIR(statbuf.st_mode))
            {
                my_list_push_front(stack, (top_type){.as_void = fullpath});
            }
            // Если это обычный файл и имя совпадает с искомым
            else if (S_ISREG(statbuf.st_mode) &&
                     str_cmp(entry->d_name, my_str_get_data(target)))
            {
                printf("%s\n", my_str_get_data(fullpath));
                my_str_destroy(fullpath);
            }
            else
            {
                my_str_destroy(fullpath);
            }
        }

        closedir(dir);
        my_str_destroy(current_path);
    }

    my_list_destroy(stack);
}

int main(int argc, const char *argv[])
{
    my_string *start_dir = my_str_create(".");
    
    for(int i = 1; i < argc; ++i)
    {
        my_string *target = my_str_create(argv[i]);
        search_file_iterative(start_dir, target);
        my_str_destroy(target);
    }
    my_str_destroy(start_dir);
    return 0;
}