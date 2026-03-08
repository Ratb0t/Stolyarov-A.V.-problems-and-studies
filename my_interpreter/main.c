#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
    exect_external,
    fork_error,
    home_env_error,
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
    int num_running_processes;
} analyzator;

int my_strcmp(const char *str1, const char *str2);

void init_analizator(analyzator *alzr);

void out_input(my_list *lst);

int format_cmd_line(analyzator *alzr, char *cmd_line[]);

int process_cd_command(analyzator *alzr, char *path);

int start_external_prog(analyzator *alzr);

int wait_startetd_process(analyzator *alzr);

int wait_startetd_process_before_quite(analyzator *alzr);

int dialog_codes_process(analyzator *alzr);

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

void set_analyzator_code(analyzator *alzr);

int dispatch_dialog_error(analyzator *alzr);

int insetr_word(analyzator *alzr);

int escape_sequence_analysis(analyzator *alzr);

int process_symbol(analyzator *alzr);

void dialog(analyzator *alzr);

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

int start_dialog();

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

void init_analizator(analyzator *alzr)
{
    alzr->ch = 0;
    alzr->prev_char = 0;
    alzr->quotes = 0;
    alzr->code = ok;
    alzr->num_running_processes = 0;
    if (!(alzr->word = my_str_create(NULL)))
        alzr->code = alloc_error;
    if (!(alzr->words = my_list_create()))
        alzr->code = alloc_error;

    return;
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

    if (alzr->code == quite)
        set_analyzator_code(alzr);

    return;
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
        if (alzr->prev_char != '\\')
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

void set_analyzator_code(analyzator *alzr)
{
    if (alzr->ch == EOF)
        alzr->code = quite;
    if (my_list_get_len(alzr->words))
        alzr->code = exect_external;//print_input;
    if (alzr->quotes)
        alzr->code = quotes_error;

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
    // printf("prev_char == %d ch == %d\n", alzr->prev_char, alzr->ch);
    if (alzr->prev_char == '\\' && alzr->ch != '\\' && alzr->ch != '\"')
    {
        if (alzr->ch != '\n')
            clear_stdin('\n');
        alzr->code = backslash_error;
    }

    if (alzr->prev_char == '\\' && alzr->ch == '\\')
        return 0;

    return alzr->ch;
}

int start_external_prog(analyzator *alzr)
{
    char **cmd_line = malloc(sizeof(char *) * (my_list_get_len(alzr->words) + 1));
    if(!cmd_line)
    {
        alzr->code = alloc_error;
        return 0;
    }

    format_cmd_line(alzr, cmd_line);
    
    if(my_strcmp(cmd_line[0], "cd") == compare_equal)
    {
        int code = process_cd_command(alzr, cmd_line[1]);
        free(cmd_line);
        return code;
    }
    else
    {
        int pid = fork();
        if(pid == 0)
        {
            execvp(cmd_line[0], cmd_line);
            perror("Command error");
            end_dialog(alzr);
            free(cmd_line);
            exit(1);
        }
        if(pid < 0)
        {
            alzr->code = fork_error;
            alzr->num_running_processes -= 1;
        }
        alzr->num_running_processes += 1;
    }
    
    free(cmd_line);
    return 1;    
}

int process_cd_command(analyzator *alzr, char *path)
{
    if (path && path[0] == '.')
    {
        if(path[1] == '\0')
            return 1;
        if(path[1] == '\\')
            path += 2;
    }

    my_string *home_dir = my_str_create(NULL);
    if (!path || path[0] == '~')
    {
        my_str_set_string(home_dir, getenv("HOME"));
        if(path)
            path+=1;
        if (my_str_get_data(home_dir) == NULL)
        {
            alzr->code = home_env_error;
            my_str_destroy(home_dir);
            return 0;
        }
    }
    my_string *path_dir = my_str_create(path);
    my_string *full_path = my_str_concatenation(home_dir, path_dir);

    if (chdir(my_str_get_data(full_path)) == -1)
        perror("cd");

    my_str_destroy(full_path);
    my_str_destroy(home_dir);
    my_str_destroy(path_dir);
    return 1;
}

/*На данный момент времени не нужна*/
int wait_startetd_process_before_quite(analyzator *alzr)
{
    while (alzr->num_running_processes)
    {
        wait(NULL);
        alzr->num_running_processes -= 1;
    }
    return 1;
}

/*На данный момент времени не нужна*/
int wait_startetd_process(analyzator *alzr)
{
    while (wait4(-1, NULL, WNOHANG, NULL) > 0)
    {
        alzr->num_running_processes -= 1;
    }
    return 1;
}

int format_cmd_line(analyzator *alzr, char *cmd_line[])
{
    my_list_iterator it = my_list_get_first(alzr->words);
    int i = 0;
    for (; it; ++i, it = it->next)
    {
        cmd_line[i] = (char *)my_str_get_data((my_string *)(it->data_holder.as_void));
    }
    cmd_line[i] = NULL;
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

int my_strcmp(const char *str1, const char *str2)
{
    int i = 0;
    for(; str1[i] && str2[i]; ++i)
    {
        if(str1[i] != str2[i])
            break;
    }
    if(str1[i] == str2[i])
        return compare_equal;
    if(str1[i] > str2[i])
        return compare_great;
    
    return compare_less;
}