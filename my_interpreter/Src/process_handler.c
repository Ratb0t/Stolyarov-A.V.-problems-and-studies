#include "analyzator.h"
#include "process_handler.h"

static sig_atomic_t fg_pid;
static void cleaning_background_processes(int sig);


extern int end_dialog(context *cnt);
extern void destroy_context(context *cnt);

static int format_cmd_line(context *cnt, char *cmd_line[]);
static int process_cd_command(context *cnt, char *path);

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
        /*ph->fg_pid = 0;
        ph->num_running_processes = 0;*/
        ph->need_init = 0;
        ph->input_redirection = NULL;
        ph->output_redirection = NULL;
        signal(SIGCHLD, cleaning_background_processes);
    }
    return;
}

static int format_cmd_line(context *cnt, char *cmd_line[])
{
    my_list_iterator it = my_list_get_first(cnt->alzr->words);
    int i = 0;
    my_string *str;
    for (; it; ++i, it = it->next)
    {
        str = (my_string *)(it->data_holder.as_void);
        if (str != cnt->proc_hanler->input_redirection && 
            str != cnt->proc_hanler->output_redirection)
        {
            cmd_line[i] = (char *)my_str_get_data(str);
        }
        else
            cmd_line[i] = NULL;
    }
    cmd_line[i] = NULL;
    return 1;
}

static int process_cd_command(context *cnt, char *path)
{
    /*Cd не является отдельным процессом*/
    cnt->code.minore_code.codes.fg_process = 0;
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
            cnt->code.major_code = home_env_error;
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

static int get_fd_for_redirection_io(context *cnt, int *fd_in, int *fd_out)
{
    if (cnt->code.minore_code.codes.in_redirect)
    {
        const char *filename = my_str_get_data(cnt->proc_hanler->input_redirection);
        cnt->proc_hanler->input_redirection = NULL;
        *fd_in = open(filename, O_RDONLY, 0666);
        if (*fd_in == -1)
        {
            perror(filename);
            return 0;
        }
    }
    if(cnt->code.minore_code.codes.out_redirect)
    {
        int open_flags = O_CREAT | O_WRONLY;
        if (cnt->code.minore_code.codes.out_redirect == truncate_file)
            open_flags |= O_TRUNC;
        else
            open_flags |= O_APPEND;

        const char *filename = my_str_get_data(cnt->proc_hanler->output_redirection);
        cnt->proc_hanler->output_redirection = NULL;
        *fd_out = open(filename, open_flags, 0666);
        if (*fd_out == -1)
        {
            perror(filename);
            return 0;
        }
    }
    return 1;
}

static void run_target_program(context *cnt, char *cmd_line[], int fd_in, int fd_out)
{
    if (cnt->code.minore_code.codes.in_redirect)
    {
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    if (cnt->code.minore_code.codes.out_redirect)
    {
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
    execvp(cmd_line[0], cmd_line);
    perror("Command error");
    free(cmd_line);
    destroy_context(cnt);
    //putchar('>');
    exit(1);
    return;
}

static void spawn_child_process(context *cnt, char *cmd_line[], int fd_in, int fd_out)
{
    int pid = fork();
    if (pid == 0)
    {
        run_target_program(cnt, cmd_line, fd_in, fd_out);
    }
    if (pid == -1)
    {
        cnt->code.major_code = fork_error;
        /*cnt->proc_hanler->num_running_processes -= 1;*/
    }
    /*cnt->proc_hanler->num_running_processes += 1;
    if (cnt->code.minore_code.codes.fg_process)
        cnt->proc_hanler->fg_pid = pid;*/
    
    if (cnt->code.minore_code.codes.fg_process)
        fg_pid = pid;
    return;
}

int start_external_prog(context *cnt)
{
    int fd_in = -1, fd_out = -1;
    int code = 1;
    char **cmd_line = malloc(sizeof(char *) * (my_list_get_len(cnt->alzr->words) + 1));
    if (!cmd_line)
    {
        cnt->code.major_code = alloc_error;
        return 0;
    }

    format_cmd_line(cnt, cmd_line);

    if (my_strcmp(cmd_line[0], "cd") == compare_equal)
    {
        code = process_cd_command(cnt, cmd_line[1]);
    }
    else
    {
        code = get_fd_for_redirection_io(cnt, &fd_in, &fd_out);
        if(code)
        {
            spawn_child_process(cnt, cmd_line, fd_in, fd_out);

            if(cnt->code.minore_code.codes.in_redirect && fd_in != -1)
                close(fd_in);
            if(cnt->code.minore_code.codes.out_redirect && fd_out != -1)
                close(fd_out);
        }
    }

    free(cmd_line);
    return code;
}

/*На данный момент времени не нужна*/
int wait_startetd_process_before_quite(context *cnt)
{
    return 1;
    // printf("Wait child process, before \n");
    // while (cnt->proc_hanler->num_running_processes) // while (wait(NULL))
    // {
    //     if (wait4(-1, NULL, WNOHANG, NULL) > 0)
    //         cnt->proc_hanler->num_running_processes -= 1;
    // }
    // return 1;
}


static void cleaning_background_processes(int sig)
{
    signal(SIGCHLD, cleaning_background_processes);
    int p;
    while ((p = wait4(-1, NULL, WNOHANG, NULL)) > 0)
    {
        if(p == fg_pid)
            fg_pid = 0;
        /*cnt->proc_hanler->num_running_processes -= 1;*/
        /*write(STDOUT_FILENO, "cleaning_bg\n", 12);*/
    }
    return;
}

int wait_foreground_process(context *cnt)
{
    if (cnt->code.minore_code.codes.fg_process && fg_pid)
    {
        /*printf("wait_fg_process %d\n", fg_pid);*/
        signal(SIGCHLD, SIG_DFL);
        int p;
        /*while((p = wait(NULL)) > 0 && p != cnt->proc_hanler->fg_pid)
        {
            cnt->proc_hanler->num_running_processes -= 1;/
        }
        cnt->proc_hanler->num_running_processes -= 1;*/
        while ((p = wait(NULL)) > 0 && p != fg_pid)
        {
            /*printf("waited %d\n", p);*/
        }
        /*printf("waited %d\n", p);*/
        fg_pid = 0;
        signal(SIGCHLD, cleaning_background_processes);
    }
    
    return 1;
}