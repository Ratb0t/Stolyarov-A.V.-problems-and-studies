#include "main.h"
/*
Чтобы по 100 раз не описывать одно и тоже
я сделал 2 библиотеки my_list и my_string

my_list - простенький односвязный список

my_string - фактически реализация вектора, который
содержит указатель на динамически выделенный буфер
*/

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

int main()
{    
    while(start_dialog()){}

    putchar('\n');
    return 0;
}

int start_dialog()
{
    analyzator alzr;
    init_analizator(&alzr);
    int code;
    if (alzr.code != ok)
    {
        code = dispatch_dialog_error(&alzr);
    }
    else
    {
        dialog(&alzr);
        code = dialog_codes_process(&alzr);
    }

    end_dialog(&alzr);
    return code;
}


int dispatch_dialog_error(analyzator *alzr)
{
    dialog_codes_process(alzr);
    printf("Try again? y/n: ");
    if ((getchar() | 0x20) == 'y')
    {
        clear_stdin('\n');
        return ok;
    }
    return quite;
}

void dialog(analyzator *alzr)
{
    wait(NULL);// wait_startetd_process(alzr);
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

    if (alzr->code == end_input_line)
        set_analyzator_code(alzr);

    return;
}


int dialog_codes_process(analyzator *alzr)
{
    switch (alzr->code)
    {
    case ok:
        return 1;
    case quite:
        wait_startetd_process_before_quite(alzr);
        return 0;
    case exect_external:
        if(!start_external_prog(alzr))
        return dialog_codes_process(alzr);
        break;
    case fork_error:
        printf("Failed to start the program. Fork");
        break;
    case home_env_error:
        printf("The 'HOME' environment variable does not exist or does not contain the path to the home directory.");
        break;
    case print_input:
        out_input(alzr->words);
        break;
    case quotes_error:
        printf("Error: unmatched quotes");
        break;
    case alloc_error:
        printf("Error: can't alloc memory");
        break;
    case backslash_error:
        printf("Error: unknown escape sequence");
        break;
    default:
        break;
    }

    fflush(stdout);
    return 1;
}

void out_input(my_list *lst)
{
    for (my_list_iterator it = my_list_get_first(lst); it; it = it->next)
    {
        putchar('[');
        printf("%s", my_str_get_data(it->data_holder.as_void));
        putchar(']');
        putchar('\n');
    }
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