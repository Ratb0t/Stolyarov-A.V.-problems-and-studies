#ifndef REINCLUDE_PROTECTION_FUNCTIONS
#define REINCLUDE_PROTECTION_FUNCTIONS
#include <stdlib.h>

struct item
{
    int x;
    struct item *next;
};

struct item *create_list_forward(int *arr, int len);
struct item *create_list_backward(int *arr, int len);
int list_len(struct item *lst);

int destroy_list(struct item *lst);
int reverse_lst(struct item **lst);

int *create_arr_with_list_len(struct item *lst);

int *create_arr_with_list_recursion_backward(struct item *lst);
int *create_arr_with_list_recursion_forward(struct item *lst);

int destroy_arr_with_list(int *arr);

#endif //! REINCLUDE_PROTECTION_FUNCTIONS