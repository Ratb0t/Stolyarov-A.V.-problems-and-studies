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
    my_list *lst;
    if (!(lst = my_list_create()))
        return 0;
    int result = 0;
    int err;
    while ((err = getline_int(&result, &readed)) != EOF && err != 0)
    {
        if (readed)
        {
            my_list_iterator iter = my_list_get_first(lst);

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
                struct item* tmp;
                if (!(tmp = malloc(sizeof(struct item))))
                {
                    continue;
                }
                tmp->times = 1;
                tmp->data = result;
                my_list_push_front(lst, tmp);
            }
        }
    }
    my_list_revers(lst);
    my_list_iterator iter = my_list_get_first(lst);

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

    for (my_list_iterator iter = my_list_get_first(lst); iter; iter = iter->next)
        free(iter->data_holder);
        
    my_list_destroy(lst);
    return 1;
}

int print_most_common_numbers()
{
    int readed = 0;
    my_list *lst;
    if (!(lst = my_list_create()))
        return 0;
    int result = 0;
    int err;
    int most_common = 0;
    while ((err = getline_int(&result, &readed)) != EOF && err != 0)
    {
        if (readed)
        {
            my_list_iterator iter = my_list_get_first(lst);

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
                struct item *tmp;
                if (!(tmp = malloc(sizeof(struct item))))
                {
                    continue;
                }
                tmp->times = 1;
                tmp->data = result;
                my_list_push_front(lst, tmp);
            }
        }
    }
    my_list_revers(lst);
    my_list_iterator iter = my_list_get_first(lst);

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

    for (my_list_iterator iter = my_list_get_first(lst); iter; iter = iter->next)
        free(iter->data_holder);

    my_list_destroy(lst);
    return 1;
}

int reverse_sentence()
{
    struct my_string *str;
    my_list *lst;
    //my_list_head head;
    int ch;
    if ((str = my_str_create(0)) == 0)
        return 0;
    if ((lst = my_list_create(str)) == 0)
        return 0;
    //head = my_list_get_head(lst);
    while ((ch = getchar()) != EOF)
    {
        if(ch <= ' ')
        {
            my_str_pushback_char(str, ' ');
            if ((my_list_push_front(lst, str) == 0 || 
            (str = my_str_create(0))) == 0)
            {
                if(str)
                    my_str_destroy(str);
                my_list_iterator iter = my_list_get_first(lst);
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
        my_str_pushback_char(str, ch);
    }
    
    my_str_destroy(str);
    
    for (my_list_iterator iter = my_list_get_first(lst); iter; iter = iter->next)
    {
#ifndef DEBUG_PRINT_print_3times_numbers
        for (int i = 0; i <= my_str_get_len(iter->data_holder); ++i)
            {
                int ch = my_str_get_data(iter->data_holder)[i];
                printf("\n%3d - %3c", ch, ch);
            }
            printf("\n%s", my_str_get_data(iter->data_holder));
#else
        printf("%s", my_str_get_data(iter->data_holder));
#endif
        
        my_str_destroy(iter->data_holder);
    }
    my_list_destroy(lst);
    putchar('\n');
    return 1;
}