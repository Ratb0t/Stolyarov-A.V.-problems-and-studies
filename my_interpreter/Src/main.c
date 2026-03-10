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
    context *shell = create_and_init_context();
    if(shell == NULL)
        return 1;
    while(start_dialog(shell)){}
    destroy_context(shell);
    putchar('\n');
    return 0;
}

int start_dialog(context *shell)
{
    context_reset_analizator(shell);
    int code;
    if (shell->code != ok)
    {
        code = dispatch_dialog_error(shell);
    }
    else
    {
        dialog(shell);
        code = dialog_codes_process(shell);
    }

    end_dialog(shell);
    return code;
}


int dispatch_dialog_error(context *alzr)
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

void dialog(context *cnt)
{
    wait(NULL);// wait_startetd_process(alzr);
    putchar('>');
    while (cnt->alzr->code == ok)
    {
        cnt->alzr->ch = getchar();
        process_symbol(cnt->alzr);
        if (cnt->alzr->code == add_char)
        {
            my_str_pushback_char(cnt->alzr->word, cnt->alzr->ch);
            cnt->alzr->code = ok;
        }
    }

    cnt->code = get_analyzator_code(cnt->alzr);

    return;
}


int dialog_codes_process(context *cnt)
{
    switch (cnt->code)
    {
    case ok:
        return 1;
    case quite:
        wait_startetd_process_before_quite(cnt);
        return 0;
    case exect_external:
        if (!start_external_prog(cnt))
            return dialog_codes_process(cnt);
        break;
    case fork_error:
        printf("Failed to start the program. Fork\n");
        break;
    case home_env_error:
        printf("The 'HOME' environment variable does not exist or does not contain the path to the home directory.\n");
        break;
    case print_input:
        out_input(cnt->alzr->words);
        break;
    case alloc_error:
        printf("Error: can't alloc memory\n");
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

int end_dialog(context *cnt)
{
    if (cnt->alzr->words)
    {
        clear_list(cnt->alzr->words);
        my_list_destroy(cnt->alzr->words);
    }
    if (cnt->alzr->word)
        my_str_destroy(cnt->alzr->word);

    cnt->alzr->word = NULL;
    cnt->alzr->words = NULL;
    return 1;
}