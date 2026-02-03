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
    my_vector *v;
    my_vector_iterator iter;
    if (!(v = my_vector_create(sizeof(struct item*))))
        return 0;
    int result = 0;
    int err;
    while ((err = getline_int(&result, &readed)) != EOF && err != 0)
    {
        if (readed)
        {
            iter = my_vector_get_first(v);

            while (iter != my_vector_get_end(v))
            {
                if ((*(struct item **)(iter))->data == result)
                {
                    (*(struct item **)(iter))->times += 1;
                    break;
                }
                else
                    my_vector_iter_next(v, &iter);
            }
            if (iter == my_vector_get_end(v))
            {
                struct item* tmp;
                if (!(tmp = malloc(sizeof(struct item))))
                {
                    continue;
                }
                tmp->times = 1;
                tmp->data = result;
                my_vector_push_back(v, (top_type){.as_void = tmp});
            }
        }
    }

    iter = my_vector_get_first(v);

    if(iter)
    {
        while (iter != my_vector_get_end(v))
        {
            if ((*(struct item **)(iter))->times == 3)
            {
                printf("%d ", (*(struct item **)(iter))->data);
            }
            my_vector_iter_next(v, &iter);
        }
        putchar('\n');
    }

    for (iter = my_vector_get_first(v); iter != my_vector_get_end(v); my_vector_iter_next(v, &iter))
        free(*(struct item **)iter);

    my_vector_destroy(v);
    return 1;
}

int print_most_common_numbers()
{
    int readed = 0;
    my_vector *v;
    my_vector_iterator iter;
    if (!(v = my_vector_create(sizeof(struct item *))))
        return 0;
    int result = 0;
    int err;
    int most_common = 0;
    while ((err = getline_int(&result, &readed)) != EOF && err != 0)
    {
        if (readed)
        {
            iter = my_vector_get_first(v);

            while (iter != my_vector_get_end(v))
            {
                if ((*(struct item **)(iter))->data == result)
                {
                    (*(struct item **)(iter))->times += 1;
                    if (most_common < (*(struct item **)(iter))->times)
                        most_common = (*(struct item **)(iter))->times;
                    break;
                }
                else
                    my_vector_iter_next(v, &iter);
            }
            if (iter == my_vector_get_end(v))
            {
                struct item *tmp;
                if (!(tmp = malloc(sizeof(struct item))))
                {
                    continue;
                }
                tmp->times = 1;
                tmp->data = result;
                my_vector_push_back(v, (top_type){.as_void = tmp});
            }
        }
    }

    iter = my_vector_get_first(v);

    if (iter)
    {
        while (iter != my_vector_get_end(v))
        {
            if ((*(struct item **)(iter))->times == most_common)
            {
                printf("%d ", (*(struct item **)(iter))->data);
            }
            my_vector_iter_next(v, &iter);
        }
        putchar('\n');
    }

    for (iter = my_vector_get_first(v); iter != my_vector_get_end(v); my_vector_iter_next(v, &iter))
        free(*(struct item**)iter);

    my_vector_destroy(v);
    return 1;
}

int reverse_sentence()
{
    my_vector *lst, *str;
    my_vector_iterator iter;
    int ch;
    if ((str = my_vector_create(sizeof(char))) == 0)
        return 0;
    if ((lst = my_vector_create(sizeof(my_vector *))) == 0)
        return 0;

    while ((ch = getchar()) != EOF)
    {
        if(ch <= ' ')
        {
            my_vector_push_back(str, (top_type){.as_int = ' '});

            if ((my_vector_push_front(lst, (top_type){.as_void = str}) == 0 ||
                 (str = my_vector_create(sizeof(char)))) == 0)
            {
                if(str)
                    my_vector_destroy(str);
                iter = my_vector_get_first(lst);
                for (; iter != my_vector_get_end(lst); my_vector_iter_next(lst, &iter))
                {
                    if (*(my_vector **)iter)
                        my_vector_destroy(*(my_vector **)iter);
                }
                my_vector_destroy(lst);
                return 0;
            }
            continue;
        }
        my_vector_push_back(str, (top_type){.as_int = ch});
    }
    
    //my_list_push_front(lst, (top_type){.as_void = str});

    my_vector_destroy(str);

    for (iter = my_vector_get_first(lst); iter != my_vector_get_end(lst); my_vector_iter_next(lst, &iter))
    {
        for (my_vector_iterator cur = my_vector_get_first(*(my_vector **)iter);
             cur != my_vector_get_end(*(my_vector **)iter); my_vector_iter_next(*(my_vector **)iter, &cur))
        {
            putchar(*(char*)cur);
        }

        my_vector_destroy(*(my_vector **)iter);
    }
    my_vector_destroy(lst);
    putchar('\n');
    return 1;
}

int print_coloumn_sentence()
{
    my_vector *lst, *str;
    my_vector_iterator iter;

    int ch, most_longer_word = 0;
    if ((str = my_vector_create(sizeof(char))) == 0)
        return 0;
    if ((lst = my_vector_create(sizeof(my_vector *))) == 0)
        return 0;

    while ((ch = getchar()) != EOF)
    {
        if (ch <= ' ')
        {
            most_longer_word = Max(most_longer_word, my_vector_get_size(str));
            if ((my_vector_push_back(lst, (top_type){.as_void = str}) == 0 ||
                 (str = my_vector_create(sizeof(char)))) == 0)
            {
                if (str)
                    my_vector_destroy(str);
                iter = my_vector_get_first(lst);
                for (; iter != my_vector_get_end(lst); my_vector_iter_next(lst, &iter))
                {
                    if (*(my_vector **)iter)
                        my_vector_destroy(*(my_vector **)iter);
                }
                my_vector_destroy(lst);
                return 0;
            }
            continue;
        }
    
        my_vector_push_back(str, (top_type){.as_int = ch});
    }

    // my_list_push_front(lst, (top_type){.as_void = str});

    my_vector_destroy(str);

    int i = 0;
    while (most_longer_word)
    {

        for (iter = my_vector_get_first(lst); iter != my_vector_get_end(lst); my_vector_iter_next(lst, &iter))
        {
            my_vector_iterator cur = my_vector_get_item(*(my_vector **)iter, i);
            if (cur)
            {
                putchar(*(char *)cur);
            }
            else
                putchar(' ');
        }
        putchar('\n');
        ++i;
        --most_longer_word;
    }

    for (iter = my_vector_get_first(lst); iter != my_vector_get_end(lst); my_vector_iter_next(lst, &iter))
        my_vector_destroy(*(my_vector **)iter);

    my_vector_destroy(lst);
    putchar('\n');
    return 1;
}

int print_numbers_in_sentence()
{
    my_vector *lst, *str;
    my_vector_iterator iter;

    int ch, most_longer_word = 0;
    if ((str = my_vector_create(sizeof(char))) == 0)
        return 0;
    if ((lst = my_vector_create(sizeof(my_vector *))) == 0)
        return 0;

    int string_size = 0;
    putchar('>');
    while ((ch = getchar()) != EOF)
    {
        //S0me st1ng 1n wi24120th num28467b3rs 1234567
        if(Is_number(ch))
        {
            my_vector_push_back(str, (top_type){.as_int = ch});
            continue;
        }
        if ((string_size = my_vector_get_size(str)))
        {
            most_longer_word = Max(most_longer_word, string_size);
            if ((my_vector_push_back(lst, (top_type){.as_void = str}) == 0 ||
                 (str = my_vector_create(sizeof(char)))) == 0)
            {
                if (str)
                    my_vector_destroy(str);
                iter = my_vector_get_first(lst);
                for (; iter != my_vector_get_end(lst); my_vector_iter_next(lst, &iter))
                {
                    if (*(my_vector **)iter)
                        my_vector_destroy(*(my_vector **)iter);
                }
                my_vector_destroy(lst);
                return 0;
            }
        }
        if(ch == '\n')
        {
            for (iter = my_vector_get_first(lst); iter != my_vector_get_end(lst);)
            {
                //printf("%d \n", my_list_get_size(lst));
                string_size = (int)my_vector_get_size(*(my_vector **)iter);
                for (my_vector_iterator cur = my_vector_get_first(*(my_vector **)iter);
                     string_size == most_longer_word && cur != my_vector_get_end(*(my_vector **)iter)
                     ; )
                {
                    putchar(*(char*)cur);
                    my_vector_iter_next(*(my_vector **)iter, &cur);
                    if (cur == my_vector_get_end(*(my_vector **)iter))
                        putchar(' ');
                }
                my_vector_destroy(*(my_vector **)iter);
                my_vector_iter_next(lst, &iter);
            }
            putchar(ch); putchar('>');
            most_longer_word = 0;
            //printf("%p \n", my_list_get_first(lst));
            //printf("%p \n", my_list_get_end(lst));
        }
    }

    // my_list_push_front(lst, (top_type){.as_void = str});

    my_vector_destroy(str);
    my_vector_destroy(lst);
    putchar('\n');
    return 1;
}
