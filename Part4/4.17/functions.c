#include "functions.h"
/*Не эффективный т.к. требует каждый раз доходить в конец списка*/
struct item *create_list_forward(int *arr, int len)
{
    struct item *lst = 0;
    struct item *tmp = 0;
    for (int i = 0; i < len; ++i)
    {
        tmp = malloc(sizeof(struct item));
        tmp->x = arr[i];
        tmp->next = 0;
        if (!lst)
            lst = tmp;
        else
        {
            struct item *iter;
            for (iter = lst; iter->next; iter = iter->next)
            {
            }
            iter->next = tmp;
        }
    }
    return lst;
}

struct item *create_list_backward(int *arr, int len)
{
    struct item *lst = 0;
    struct item *tmp = 0;
    for (int i = len - 1; i > -1; --i)
    {
        tmp = malloc(sizeof(struct item));
        tmp->x = arr[i];
        tmp->next = lst;
        lst = tmp;
    }
    return lst;
}

int destroy_list(struct item *lst)
{
    while (lst)
    {
        struct item *tmp = lst;
        lst = lst->next;
        free(tmp);
    }

    return 1;
}

int reverse_lst(struct item **lst)
{
    /*Разворот списка на месте*/
    struct item *prev = 0;
    struct item *current = *lst;
    struct item *next = 0;
    while (current != 0)
    {
        // Сохраняем следующий узел
        next = current->next;

        // Меняем направление указателя
        current->next = prev;

        // Сдвигаем указатели
        prev = current;
        current = next;
    }
    *lst = prev;
    return 1;
}

int list_len(struct item *lst)
{
    int len = 0;
    while (lst)
    {
        ++len;
        lst = lst->next;
    }

    return len;
}

int *create_arr_with_list_len(struct item *lst)
{
    int lst_len = list_len(lst);
    int *arr = malloc(sizeof(int) * (lst_len + 1));
    arr[0] = lst_len;
    for (int i = 1; i < (lst_len + 1) && lst; lst = lst->next, ++i)
    {
        arr[i] = lst->x;
    }
    return arr;
}

struct item *create_list_recursion(int *arr, int len)
{
    struct item *tmp = malloc(sizeof(struct item));
    tmp->x = *arr;
    if (len - 1)
        tmp->next = create_list_recursion(arr + 1, len - 1);
    else
        tmp->next = 0;

    return tmp;
}

static int create_arr_with_list_recursion_work_forward(int *arr, struct item *lst)
{
    if(lst->next)
        *arr = create_arr_with_list_recursion_work_forward(arr + 1, lst->next);
    return lst->x;
}

int *create_arr_with_list_recursion_forward(struct item *lst)
{
    int lst_len = list_len(lst);
    int *arr = malloc(sizeof(int) * (lst_len + 1));
    arr[0] = lst_len;
    arr += 1;
    *arr = create_arr_with_list_recursion_work_forward(arr + 1, lst);

    return (arr - 1);
}

static void create_arr_with_list_recursion_work(int *arr, struct item *lst)
{
    if (!lst)
        return;
    *arr = lst->x;
    create_arr_with_list_recursion_work(arr - 1, lst->next);
    return;
}
int *create_arr_with_list_recursion_backward(struct item *lst)
{
    int lst_len = list_len(lst);
    int *arr = malloc(sizeof(int) * (lst_len + 1));
    for (int i = 1; i < lst_len + 1; ++i)
        arr[i] = 100;
    arr[0] = lst_len;
    arr += lst_len;
    create_arr_with_list_recursion_work(arr, lst);

    return (arr - lst_len);
}

int destroy_arr_with_list(int *arr)
{
    free(arr);
    return 1;
}