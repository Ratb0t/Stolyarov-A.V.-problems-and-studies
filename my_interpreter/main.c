#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "my_string.h"
#include "my_list.h"
#include "my_types.h"
/*
Чтобы по 100 раз не описывать одно и тоже
я сделал 2 библиотеки my_list и my_string

my_list - простенький односвязный список

my_string - фактически реализация вектора, который
содержит указатель на динамически выделенный буфер
*/

enum codes
{
    quite,
    ok,
    print_input,
    quotes_error,
    alloc_error,
    backslash_error
};

void out_input(my_list *lst)
{
    for(my_list_iterator it = my_list_get_first(lst); it; it = it->next)
    {
        putchar('[');
        printf("%s", my_str_get_data(it->data_holder.as_void));
        putchar(']');
        putchar('\n');
    }
    return;
}

int codes_handler(int code, my_list *lst)
{
    switch (code)
    {
    case ok:
        return 1;
    case quite:
        return 0;
    case print_input:
        out_input(lst);
        break;
    case quotes_error:
        printf("Error: unmatched quotes\n");
        break;
    case alloc_error:
        printf("Error: can't alloc memory\n");
        break;
    case backslash_error:
        printf("Error: unknown escape sequence\n");
        break;
    default:
        break;
    }
    return 1;
}

void clear_list(my_list *lst)
{
    my_list_iterator it;
    while((it = my_list_get_first(lst)))
    {
        my_str_destroy(it->data_holder.as_void);
        my_list_delete_item(lst, 0);
    }

    return;
}

void clear_stdin(int ch)
{
    
    while (getchar() != ch)
    {
    }
    return;
}

int dialog(my_string **word, my_list *words)
{
    int code = ok;
    int ch, prev_char = 0;
    int quotes = 0;
    putchar('>');
    while (prev_char != '\n' && (ch = getchar()) != EOF)
    {
        if(prev_char == '\\' && (ch != '\\' || ch != '\"'))
        {
            if(ch != '\n')
                clear_stdin('\n');
            return backslash_error;
        }

        switch (ch)
        {
        case '\"':
        {
            if(prev_char == '\\')
                break;
            ++quotes;
            continue;
        }
            break;
        case '\\':
            if(prev_char != '\\')
            {
                prev_char = ch;
                continue;
            }
            break;
        case '\t':
        case ' ':
        case '\n':
        {
            if (!(quotes & 1))
            {
                if (my_str_get_len(*word) || quotes)
                {
                    my_list_push_back(words, (top_type){.as_void = *word});
                    *word = my_str_create(0);
                    if(!word)
                    {
                        if (ch != '\n')
                            clear_stdin('\n');
                        return alloc_error;
                    }
                }
                quotes = 0;
                prev_char = ch;
                continue;
            }
        }
            break;
        default:
            break;
        }
        prev_char = ch;

        my_str_pushback_char(*word, ch);
    }
    
    if(ch == EOF) code = quite;
    if(my_list_get_len(words)) code = print_input;
    if (quotes) code = quotes_error;

    return code;
}

int end_dialog(my_string **word, my_list **words)
{
    clear_list(*words);
    my_str_destroy(*word);
    my_list_destroy(*words);
    *word = NULL;
    *words = NULL;
    return 1;
}

int start_dialog()
{
    my_string *word = my_str_create(NULL);
    my_list *words = my_list_create();
    
    int code = codes_handler(dialog(&word, words), words);

    end_dialog(&word, &words);
    return code;
}

int main()
{    
    while(start_dialog()){}

    return 0;
}
