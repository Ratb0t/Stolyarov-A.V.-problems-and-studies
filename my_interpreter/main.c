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
    add_char,
    print_input,
    quotes_error,
    alloc_error,
    backslash_error,
    start_dialog_error
};

typedef struct analyzator
{
    my_string *word;
    my_list *words;
    int ch;
    int prev_char;
    int quotes;
    int code;
} analyzator;

void init_analizator(analyzator *alzr)
{
    alzr->ch = 0;
    alzr->prev_char = 0;
    alzr->quotes = 0;
    alzr->code = ok;
    if(!(alzr->word = my_str_create(NULL)))
        alzr->code = alloc_error;
    if(!(alzr->words = my_list_create()))
        alzr->code = alloc_error;

    return;
}

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

int dialog_codes_handler(analyzator *alzr)
{
    switch (alzr->code)
    {
    case ok:
        return 1;
    case quite:
        return 0;
    case print_input:
        out_input(alzr->words);
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

void set_analyzator_code(analyzator *alzr)
{
    if (alzr->ch == EOF)
        alzr->code = quite;
    if (my_list_get_len(alzr->words))
        alzr->code = print_input;
    if (alzr->quotes)
        alzr->code = quotes_error;

    return;
}

int dispatch_start_dialog_error(analyzator *alzr)
{
    dialog_codes_handler(alzr);
    printf("Try again? y/n: ");
    if((getchar() | 0x20) == 'y')
    {
        clear_stdin('\n');
        return ok;
    }
    return quite;
}

int insetr_word(analyzator *alzr)
{
    if (!(alzr->quotes & 1))
    {
        if (my_str_get_len(alzr->word) || alzr->quotes)
        {
            my_list_push_back(alzr->words, (top_type){.as_void = alzr->word});
            alzr->word = my_str_create(0);
            if (!alzr->word)
            {
                if (alzr->ch != '\n')
                    clear_stdin('\n');
                alzr->code = alloc_error;
                return quite;
            }
        }
        alzr->quotes = 0;
    }
    return ok;
}

int escape_sequence_analysis(analyzator *alzr)
{
    //printf("prev_char == %d ch == %d\n", alzr->prev_char, alzr->ch);
    if (alzr->prev_char == '\\' && alzr->ch != '\\' && alzr->ch != '\"')
    {
        if (alzr->ch != '\n')
            clear_stdin('\n');
        alzr->code = backslash_error;
    }

    if(alzr->prev_char == '\\' && alzr->ch == '\\')
        return 0;

    return alzr->ch;
}

int process_symbol(analyzator *alzr)
{
    alzr->code = add_char;
    switch (alzr->ch)
    {
    case EOF:
        alzr->code = quite;
        break;
    case '\"':
        if (alzr->prev_char != '\\')
        {   
            alzr->code = ok;
            alzr->quotes += 1;
        }
    break;
    case '\\':
        if(alzr->prev_char != '\\')
            alzr->code = ok;
        break;
    case '\n':
        alzr->code = quite;
        insetr_word(alzr);
        break;
    case '\t':
    case ' ':
        if (!(alzr->quotes & 1))
            alzr->code = ok;
        insetr_word(alzr);
        break;
    default:
        break;
    }

    alzr->prev_char = escape_sequence_analysis(alzr);

    return ok;
}

void dialog(analyzator *alzr)
{
    putchar('>');
    while (alzr->code == ok)
    {
        alzr->ch = getchar();
        process_symbol(alzr);
        if (alzr->code == add_char)
        {
            my_str_pushback_char(alzr->word, alzr->ch);
            alzr->code = ok;
        }
    }

    if(alzr->code == quite)
        set_analyzator_code(alzr);

    return;
}

int end_dialog(analyzator *alzr)
{
    if (alzr->words)
    {
        clear_list(alzr->words);
        my_list_destroy(alzr->words);
    }
    if (alzr->word)
        my_str_destroy(alzr->word);
    
        
    alzr->word = NULL;
    alzr->words = NULL;
    return 1;
}

int start_dialog()
{
    analyzator alzr;
    init_analizator(&alzr);
    int code;
    if (alzr.code != ok)
    {
        code = dispatch_start_dialog_error(&alzr);
    }
    else
    {
        dialog(&alzr);
        code = dialog_codes_handler(&alzr);
    }

    end_dialog(&alzr);
    return code;
}

int main()
{    
    while(start_dialog()){}

    putchar('\n');
    return 0;
}
