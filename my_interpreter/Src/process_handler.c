#include "analyzator.h"
#include "process_handler.h"

extern int end_dialog(analyzator *alzr);

process_handle *create_process_handle()
{
    process_handle *ph = malloc(sizeof(process_handle));
    if(ph == NULL)
        printf("Interpeter create error: Failed to allocate process_handler\n");
    else
        ph->need_init = 1;
    return ph;
}

void destroy_process_handle(process_handle *ph)
{
    free(ph);
    return;
}

void init_process_handle(process_handle *ph)
{
    if(ph->need_init)
    {
        ph->fg_pid = 0;
        ph->num_running_processes = 0;
        ph->need_init = 0;
    }
    return;
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

int process_cd_command(analyzator *alzr, char *path)
{
    if (path && path[0] == '.')
    {
        if (path[1] == '\0')
            return 1;
        if (path[1] == '\\')
            path += 2;
    }

    my_string *home_dir = my_str_create(NULL);
    if (!path || path[0] == '~')
    {
        my_str_set_string(home_dir, getenv("HOME"));
        if (path)
            path += 1;
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

int start_external_prog(analyzator *alzr)
{
    char **cmd_line = malloc(sizeof(char *) * (my_list_get_len(alzr->words) + 1));
    if (!cmd_line)
    {
        alzr->code = alloc_error;
        return 0;
    }

    format_cmd_line(alzr, cmd_line);

    if (my_strcmp(cmd_line[0], "cd") == compare_equal)
    {
        int code = process_cd_command(alzr, cmd_line[1]);
        free(cmd_line);
        return code;
    }
    else
    {
        int pid = fork();
        if (pid == 0)
        {
            execvp(cmd_line[0], cmd_line);
            perror("Command error");
            end_dialog(alzr);
            free(cmd_line);
            exit(1);
        }
        if (pid < 0)
        {
            alzr->code = fork_error;
            //alzr->num_running_processes -= 1;
        }
        //alzr->num_running_processes += 1;
    }

    free(cmd_line);
    return 1;
}

/*На данный момент времени не нужна*/
int wait_startetd_process_before_quite(analyzator *alzr)
{
    // while (alzr->num_running_processes && wait4(-1, NULL, WNOHANG, NULL) > 0) // while (wait(NULL))
    // {
    //     alzr->num_running_processes -= 1;
    // }
    return 1;
}

/*На данный момент времени не нужна*/
int cleaning_background_processes(analyzator *alzr)
{
    // while (wait4(-1, NULL, WNOHANG, NULL) > 0)
    // {
    //     alzr->num_running_processes -= 1;
    // }
    return 1;
}

int wait_foreground_process(analyzator *alzr)
{
    //int p;

    return 0;
}