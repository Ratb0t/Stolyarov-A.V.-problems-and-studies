#include <stdio.h>
#include "my_list.h"

#define mod(d) (((d) >= 0) ? (d) : -(d))

typedef struct pair
{
    int v1, v2;
}pair;


int get_diff_of_five()
{
    pair p = {0, 0};
    int *buff  = (int*)&p;
    my_list *lst = my_list_create();
    int t = 0;
    int r;
    while ((r = scanf("%d", &buff[t])) != EOF)
    {
        if(!r)
        {
            /* если символ не я вляется числом, 
            scanf не уберет его из потока, по этому
            читаем его сами*/
            getchar(); 
            continue;
        }
        if (t && (mod(buff[0] - buff[1]) <= 5))
        {
            /*Так делать нельзя. Это хак, но выделять память для каждой пары, 
            а затем освобождать для такой мелкой программы лень*/
            my_list_push_front(lst, (top_type){.as_ulong = *(long*)buff});
        }
        t ^= 1;
    }

    for (my_list_iterator iter = my_list_get_first(lst); iter; iter = iter->next)
    {
        /*
        Этот хак вытекает из предыдущего т.к. ulong привести к pair нельзя, берем адрес
        этого long и приводим к pair* после чего уже спокойно обращаемся к полям
        */
        printf("%d %d\n", ((pair *)(&iter->data_holder.as_ulong))->v1, ((pair *)(&iter->data_holder.as_ulong))->v2);
    }

    my_list_destroy(lst);

    return 1;
}

int main()
{
    get_diff_of_five();
    return 0;
}