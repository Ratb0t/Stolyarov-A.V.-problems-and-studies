#include <stdlib.h>
#include "my_list.h"

struct my_list
{
    struct node *head;
    struct node *tail;
    unsigned int length;
};

my_list *my_list_create()
{
    my_list *lst;
    if (!(lst = malloc(sizeof(my_list))))
        return NULL;

    lst->head = NULL;
    lst->tail = NULL;
    lst->length = 0;
    return lst;
}

// int my_list_init(my_list *lst, void *data)
// {
//     struct node *tmp = malloc(sizeof(struct node));
//     if (!tmp)
//         return 0;
//     tmp->data_holder = data;
//     tmp->next = 0;
//     lst->head = tmp;
//     lst->tail = tmp;
//     return 1;
// }

int my_list_destroy(my_list *lst)
{
    while (lst->head)
    {
        my_list_iterator tmp = lst->head;
        lst->head = lst->head->next;
        free(tmp);
    }
    
    free(lst);
    return 1;
}

int my_list_revers(my_list *lst)
{
    my_list_iterator prev = 0;
    my_list_iterator cur = lst->head;
    my_list_iterator next = 0;
    
    lst->head = lst->tail;
    lst->tail = cur;
    
    while (cur)
    {
        next = cur->next;

        cur->next = prev;

        prev = cur;
        cur = next;
    }

    
    return 1;
}

int my_list_push_front(my_list *list, void *data)
{

    struct node *tmp;

    if ((tmp = malloc(sizeof(struct node))) == 0)
        return 0;

    tmp->data_holder = data;

    tmp->next = list->head;
    list->head = tmp;
    if(list->tail == NULL)
        list->tail = list->head;

    list->length += 1;
    return 1;
}

int my_list_push_back(my_list *list, void *data)
{

    struct node *tmp;

    if ((tmp = malloc(sizeof(struct node))) == 0)
        return 0;

    tmp->data_holder = data;

    tmp->next = NULL;
    list->tail->next = tmp;
    list->tail = tmp;

    if (list->head == NULL)
        list->head = list->tail;

    list->length += 1;
    return 1;
}

int my_list_delete_item(my_list *lst, int pos)
{
    if(pos > lst->length)
        return 0;

    my_list_iterator *iter;
    iter = &lst->head;
    
    while (pos)
    {
        iter = &((*iter)->next);
        --pos;
    }
   
    my_list_iterator tmp = *iter;
    *iter = tmp->next;
    free(tmp);

    lst->length -= 1;
    return 1;
}

inline unsigned int my_list_get_len(const my_list *lst)
{
    return lst->length;
}

my_list_iterator my_list_get_item(const my_list *lst, unsigned int pos)
{
    if(pos > lst->length)
        return NULL;
    
    my_list_iterator iter;

    for (iter = lst->head; pos && iter; --pos)
        iter = iter->next;

    return iter;
}

inline my_list_iterator my_list_get_first(const my_list *lst)
{
    return lst->head;
}
inline my_list_iterator my_list_get_last(const my_list *lst)
{
    return lst->tail;
}

inline my_list_head my_list_get_head(const my_list *lst)
{
    return (my_list_head) &(lst->head);
}
inline my_list_tail my_list_get_tail(const my_list *lst)
{
    return (my_list_tail) &(lst->tail);
}
