#include <signal.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

enum
{
    fd_read = 0,
    fd_write
};



void child_proc_writer(int pipe_fd[2], char *cmd_line[])
{
    if (fork() == 0)
    {
        close(pipe_fd[fd_read]);
        dup2(pipe_fd[fd_write], STDOUT_FILENO);
        close(pipe_fd[fd_write]);

        execvp(cmd_line[0], cmd_line);
        perror("exec");
        _exit(1);
    }

    return;
}

void child_proc_reader(int pipe_fd[2], char *cmd_line[])
{
    if (fork() == 0)
    {
        close(pipe_fd[fd_write]);
        dup2(pipe_fd[fd_read], STDIN_FILENO);
        close(pipe_fd[fd_read]);

        execvp(cmd_line[0], cmd_line);
        perror("exec");
        _exit(1);
    }

    return;
}

int main(int argc, char *argv[])
{
    int pipe_fd[2];
    pipe(pipe_fd);
    if(argc < 2)
    {
        printf("Not enough parameters\n");
        return 1;
    }
    int i;
    for (i = 0; argv[i]; ++i)
    {
        if(argv[i][0] == ';' && argv[i][1] == ';')
            break;
    }

    argv[i] = NULL;
    child_proc_writer(pipe_fd, argv + 1);
    child_proc_reader(pipe_fd, argv + i + 1);

    close(pipe_fd[0]);
    close(pipe_fd[1]);
    while(wait(NULL) > 0)
    {}

    return 0;
}
