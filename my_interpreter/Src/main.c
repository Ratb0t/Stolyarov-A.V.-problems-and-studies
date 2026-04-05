#include "main.h"
/*
Чтобы по 100 раз не описывать одно и тоже
я сделал 2 библиотеки my_list и my_string

my_list - простенький односвязный список

my_string - фактически реализация вектора, который
содержит указатель на динамически выделенный буфер
*/

int main()
{
    /* Для поиска утечек
    int *s = malloc(sizeof(int));
    free(s);
    */
    context *cnt = create_and_init_context();
    if(cnt == NULL)
        return 1;
    while(start_dialog(cnt)){}
    destroy_context(cnt);
    putchar('\n');
    return 0;
}

int start_dialog(context *cnt)
{
    wait_foreground_process(cnt);
    /*cleaning_background_processes(cnt);*/
    context_reset_analizator(cnt);
    int code;
    if (cnt->code.major_code != ok)
    {
        code = dispatch_dialog_error(cnt);
    }
    else
    {
        dialog(cnt);
        code = dialog_codes_process(cnt);
    }

    end_dialog(cnt);
    return code;
}


int dispatch_dialog_error(context *alzr)
{
    dialog_codes_process(alzr);
    printf("Try again? y/n: ");
    if ((getchar() | 0x20) == 'y')
    {
        while (getchar() != '\n')
        {}
        
        return ok;
    }
    return quite;
}

void dialog(context *cnt)
{
    putchar('>');
    while (cnt->alzr->code.major_code == ok)
    {
        cnt->alzr->ch = getchar();
        process_symbol(cnt);
        if (cnt->alzr->code.major_code == add_char)
        {
            my_str_pushback_char(cnt->alzr->lexeme->word, cnt->alzr->ch);
            cnt->alzr->code.major_code = ok;
        }
    }
    flush_stdin(cnt);
    set_control_code(cnt);

    return;
}


int dialog_codes_process(context *cnt)
{
    switch (cnt->code.major_code)
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
        printf("Failed to start the program. Fork.\n");
        break;
    case home_env_error:
        printf("The 'HOME' environment variable does not exist or does not contain the path to the home directory.\n");
        break;
    case print_input:
        out_input(cnt->alzr->lexemes);
        break;
    case alloc_error:
        printf("Error: can't alloc memory.\n");
        break;
    case not_implemented:
        printf("Future not implemented yet.\n");
        break;
    case unexpected_cymbol:
        if(cnt->lex_err.cur_lexem != NULL)
        {
            printf("Syntax error near unexpected token '%c'\n",
                   my_str_get_data(cnt->lex_err.cur_lexem->word)[0]);
        }
        else
            printf("Interpretate error\n");        
        break;
    default:
        break;
    }
    minor_code_correction_after_error(cnt);
    fflush(stdout);
    return 1;
}

void out_input(my_list *lst)
{
    for (my_list_iterator it = my_list_get_first(lst); it; it = it->next)
    {
        putchar('[');
        printf("%s", my_str_get_data(((struct lexeme *)it->data_holder.as_void)->word));
        putchar(']');
        putchar('\n');
    }
    return;
}

int end_dialog(context *cnt)
{
    clear_analyzator(cnt->alzr);
    return 1;
}

int minor_code_correction_after_error(context *cnt)
{
    if (cnt->code.major_code != exect_external)
        cnt->code.minore_code.codes.fg_process = 0;
    return 1;
}