#include <stdio.h>
#include "functions.h"


//////////////////////////////////////////////////////////////////
int main()
{
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct item *lst = create_list_backward(arr, 10);
    int *arr2 = create_arr_with_list_recursion_forward(lst);
    for (struct item *iter = lst; iter; iter = iter->next)
        printf("%d ", iter->x);
    putchar('\n');
    for (int i = 0; i < arr2[0] + 1; ++i)
        printf("%d ", arr2[i]);
    putchar('\n');

    destroy_list(lst);
    destroy_arr_with_list(arr2);
    return 0;
}
//////////////////////////////////////////////////////////////////

