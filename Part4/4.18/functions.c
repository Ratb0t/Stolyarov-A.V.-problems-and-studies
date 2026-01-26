#include "functions.h"

int list_revers(struct item **lst)
{
    struct item *prev = 0;
    struct item *cur = *lst;
    struct item *next = 0;

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

int destroy_lst(struct item *lst)
{
    while (lst)
    {
        struct item *tmp = lst;
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
    struct item *lst = 0;
    int result = 0;
    int err;
    while ((err = getline_int(&result, &readed)) != EOF && err != 0)
    {
        if (readed)
        {
            struct item *iter = lst;

            while (iter)
            {
                if (iter->data == result)
                {
                    iter->times += 1;
                    break;
                }
                else
                    iter = iter->next;
            }
            if (!iter)
            {
                iter = malloc(sizeof(struct item));
                if (!iter)
                    continue;
                iter->data = result;
                iter->times = 1;
                iter->next = lst;
                lst = iter;
            }
        }
    }
    list_revers(&lst);
    struct item *iter = lst;
    
    if(iter)
    {
        while (iter)
        {
            if (iter->times >= 3)
            {
                printf("%d ", iter->data);
            }
            iter = iter->next;
        }
        putchar('\n');
    }
    
    destroy_lst(lst);
    return 1;
}

int print_most_common_numbers()
{
    int ch;
    int readed = 0;
    struct item *lst = 0;
    int result = 0;
    while ((ch = getchar()) != EOF)
    {

        if (ch >= '0' && ch <= '9')
        {
            result *= 10;
            result += ch - '0';
            readed += 1;
        }
        if (readed && (ch == ' ' || ch == '\t' || ch == '\n'))
        {
            struct item *iter = lst;

            while (iter)
            {
                if (iter->data == result)
                {
                    iter->times += 1;
                    break;
                }
                else
                    iter = iter->next;
            }
            if (!iter)
            {
                iter = malloc(sizeof(struct item));
                if (!iter)
                    continue;
                iter->data = result;
                iter->times = 1;
                iter->next = lst;
                lst = iter;
            }

            readed = 0;
            result = 0;
        }
    }
    list_revers(&lst);
    struct item *iter = lst;
    while (iter)
    {
        if (iter->times >= 3)
        {
            printf("%d ", iter->data);
        }
        iter = iter->next;
    }
    putchar('\n');
    destroy_lst(lst);
    return 1;
}

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
        if(*readed)
            *out = result;
        return EOF;
    // }
        
    // return 0;
}