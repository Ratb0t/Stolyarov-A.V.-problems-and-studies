#include "functions.h"

int getline_int(int *out, int *readed)
{
    int ch;
    int result = 0;
    int sign = 0;
    *readed = 0;
    while ((ch = getchar()) != EOF)
    {
        if (ch == '-')
            sign = 1;

        if (ch >= '0' && ch <= '9')
        {
            if (result > __INT_MAX__ / 10 || result < (~__INT_MAX__) / 10)
            {
#ifdef DEBUG_PRINT_print_3times_numbers
                printf("Overflow.\n");
#endif
                *readed = -21;
                return 0;
            }
            result *= 10;
            if (!sign)
            {
                if (check_overflow(result, ch - '0')) // if (result > __INT_MAX__ - (ch - '0') )
                {
#ifdef DEBUG_PRINT_print_3times_numbers
                    printf("Too large a number was entered, causing an overflow.\n");
#endif
                    *readed = -22;
                    return 0;
                }
                result += ch - '0';
            }
            else
            {
                if (check_overflow(result, -(ch - '0'))) // if (result < (~__INT_MAX__) - (ch - '0') )
                {
#ifdef DEBUG_PRINT_print_3times_numbers
                    printf("Too small a number was entered, causing an overflow.\n");
#endif
                    *readed = -23;
                    return 0;
                }
                result -= ch - '0';
            }

            *readed += 1;
        }
        if (*readed && (ch == ' ' || ch == '\t' || ch == '\n'))
        {
            *out = result;
            return 1;
        }
    }

    // if (ch == EOF)
    // {
    if (*readed)
        *out = result;
    return EOF;
    // }

    // return 0;
}

int list_revers(struct node **lst)
{
    struct node *prev = 0;
    struct node *cur = *lst;
    struct node *next = 0;

    while (cur)
    {
        next = cur->next;

        cur->next = prev;

        prev = cur;
        cur = next;
    }

    *lst = prev;
    return 1;
}

int my_list_destroy(struct node *lst)
{
    while (lst)
    {
        struct node *tmp = lst;
        lst = lst->next;
        free(tmp);
    }

    return 1;
}


int test_overflow()
{
    printf("0 == %d\n", check_overflow(~__INT_MAX__, __INT_MAX__)); // 0
    printf("1 == %d\n", check_overflow(~__INT_MAX__, -1));          // 1
    printf("1 == %d\n", check_overflow(__INT_MAX__, __INT_MAX__));  // 1
    printf("0 == %d\n", check_overflow(1000, 2000));                // 0
    printf("1 == %d\n", check_overflow(__INT_MAX__, 1));            // 1
    printf("1 == %d\n", check_overflow(~__INT_MAX__, -1));          // 1
    printf("1 == %d\n", check_overflow((~__INT_MAX__) + 8, -9));    // 1
    return 1;
}

int print_3times_numbers()
{
    int readed = 0;
    struct node *lst = 0;
    int result = 0;
    int err;
    while ((err = getline_int(&result, &readed)) != EOF && err != 0)
    {
        if (readed)
        {
            struct node *iter = lst;

            while (iter)
            {
                if (((struct item *)(iter->data_holder))->data == result)
                {
                    ((struct item *)(iter->data_holder))->times += 1;
                    break;
                }
                else
                    iter = iter->next;
            }
            if (!iter)
            {
                iter = malloc(sizeof(struct node));
                if (!iter)
                    continue;
                iter->data_holder = malloc(sizeof(struct item));
                if (!iter->data_holder)
                {
                    free(iter);
                    continue;
                }
                ((struct item *)(iter->data_holder))->times = 1;
                ((struct item *)(iter->data_holder))->data = result;
                iter->next = lst;
                lst = iter;
            }
        }
    }
    list_revers(&lst);
    struct node *iter = lst;
    
    if(iter)
    {
        while (iter)
        {
            if (((struct item *)(iter->data_holder))->times == 3)
            {
                printf("%d ", ((struct item *)(iter->data_holder))->data);
            }
            iter = iter->next;
        }
        putchar('\n');
    }
    

    for(struct node *iter = lst; iter; iter = iter->next)
        free(iter->data_holder);
        
    my_list_destroy(lst);
    return 1;
}

int print_most_common_numbers()
{
    int readed = 0;
    struct node *lst = 0;
    int result = 0;
    int err;
    int most_common = 0;
    while ((err = getline_int(&result, &readed)) != EOF && err != 0)
    {
        if (readed)
        {
            struct node *iter = lst;

            while (iter)
            {
                if (((struct item *)(iter->data_holder))->data == result)
                {
                    ((struct item *)(iter->data_holder))->times += 1;
                    if (most_common < ((struct item *)(iter->data_holder))->times)
                        most_common = ((struct item *)(iter->data_holder))->times;
                    break;
                }
                else
                    iter = iter->next;
            }
            if (!iter)
            {
                iter = malloc(sizeof(struct node));
                if (!iter)
                    continue;
                iter->data_holder = malloc(sizeof(struct item));
                if (!iter->data_holder)
                {
                    free(iter);
                    continue;
                }
                ((struct item *)(iter->data_holder))->times = 1;
                ((struct item *)(iter->data_holder))->data = result;
                iter->next = lst;
                lst = iter;
            }
        }
    }
    list_revers(&lst);
    struct node *iter = lst;

    if (iter)
    {
        while (iter)
        {
            if (((struct item *)(iter->data_holder))->times == most_common)
            {
                printf("%d ", ((struct item *)(iter->data_holder))->data);
            }
            iter = iter->next;
        }
        putchar('\n');
    }

    for (struct node *iter = lst; iter; iter = iter->next)
        free(iter->data_holder);

    my_list_destroy(lst);
    return 1;
}

struct node *my_list_create(void *data)
{
    struct node *tmp = malloc(sizeof(struct node));
    if (!tmp)
        return NULL;

    tmp->data_holder = data;
    
    tmp->next = 0;

    return tmp;
}
int my_list_pushfoward(struct node **list, void *data)
{
    
    struct node *tmp;

    if ((tmp = malloc(sizeof(struct node))) == 0)
        return 0;

    tmp->data_holder = data;

    tmp->next = *list;
    *list = tmp;

    return 1;
}
int reverse_sentence()
{
    struct my_string *str;
    struct node *lst;
    int ch;
    if ((str = my_str_create(0)) == 0)
        return 0;
    if ((lst = my_list_create(str)) == 0)
        return 0;
    while ((ch = getchar()) != EOF)
    {
        if(ch <= ' ')
        {
            my_str_pushback_char(lst->data_holder, ' ');
            my_str_pushback_char(lst->data_holder, 0);
            if ((str = my_str_create(0)) == 0 ||
                my_list_pushfoward(&lst, str) == 0)
            {
                if(str)
                    my_str_destroy(str);
                struct node *iter = lst;
                for (; iter; iter = iter->next)
                {
                    if(iter->data_holder)
                        my_str_destroy(iter->data_holder);
                }
                my_list_destroy(lst);
                return 0;
            }
            continue;
        }
        my_str_pushback_char(lst->data_holder, ch);
    }

    for (struct node *iter = lst; iter; iter = iter->next)
    {
        printf("%s", my_str_get_data(iter->data_holder));
        my_str_destroy(iter->data_holder);
    }
    my_list_destroy(lst);
    putchar('\n');
    return 1;
}