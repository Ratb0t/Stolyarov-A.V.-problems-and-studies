#include "my_string.h"
#include "my_list.h"
#include <assert.h>
#include <stdio.h>

int test_str()
{

    printf("%s\n", VERY_LONG_STRING);

    struct my_string *str1 = my_str_create("Hello saivor");
    struct my_string *str2 = my_str_create(" world ");
    
    assert(str1 != 0);

    printf("%s\n", *(char **)str1);

    // // assert(set_string(str1, VERY_LONG_STRING) == 1);
    printf("%s\n", *(char **)str2);

    struct my_string *result1 = my_str_insert(str1, 5, str2);
    printf("%s\n", my_str_get_data(result1));
    my_str_destroy(result1);

    struct my_string *result2 = my_str_concatenation(str1, str2);
    printf("%s\n", my_str_get_data(result2));
    my_str_pushback_char(result2, 's');
    my_str_pushback_char(result2, '\n');
    printf("%s\n", my_str_get_data(result2));
    my_str_destroy(result2);

    my_str_destroy(str1);
    my_str_destroy(str2);
    


    return 1;
}

int test_list()
{
    my_list *lst = my_list_create();
    for(int i = 0; i <= 20; ++i)
        my_list_push_front(lst, (top_type){.as_int = i});


    my_list_tail tail = my_list_get_tail(lst);
    my_list_head head = my_list_get_tail(lst);
    for (int i = 0; i <= 20; ++i)
        printf("%d\n",my_list_delete_item(lst, my_list_get_len(lst) - 1).as_int);

    printf("%p\n", *tail);
    printf("%p\n", *head);

    my_list_destroy(lst);

    return 1;
}

int main()
{

    test_list();

    return 0;
}