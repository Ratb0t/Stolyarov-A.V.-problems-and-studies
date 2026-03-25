#include <signal.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

sig_atomic_t readers;

void handler(int sig)
{
    signal(SIGPIPE, handler);
    --readers;
    return;
}

enum
{
    fd_read = 0,
    fd_write
};

void child_proc(int pipe_fd[2], char *cmd_line[])
{
    ++readers;
    if (fork() == 0)
    {
        /*printf("kill -s SIGINT %d\n", getpid());
        printf("kill -s SIGTERM %d\n", getpid());
        printf("kill -s SIGKILL %d\n", getpid());*/
        close(pipe_fd[fd_write]);

        dup2(pipe_fd[fd_read], STDIN_FILENO);
        close(pipe_fd[fd_read]);

        execvp(cmd_line[0], cmd_line);
        perror("exec");
        _exit(1);
    }
    
    return;
}

void write_to_child(int pipe_fd[2])
{
    char buf[16];
    int written;
    for (int i = 1; i <= 1000000 && readers; ++i)
    {
        written = sprintf(buf, "%d\n", i);
        write(pipe_fd[fd_write], buf, written);
    }

    return;    
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Not enough parameters\n");
        return 1;
    }

    readers = 0;
    signal(SIGPIPE, handler);

    int status;

    int pipe_fd[2];
    pipe(pipe_fd);
    

    child_proc(pipe_fd, argv + 1);
    close(pipe_fd[fd_read]);

    write_to_child(pipe_fd);
    close(pipe_fd[fd_write]);

    if (wait(&status) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Exited %d\n", WEXITSTATUS(status));
        }
        if(WIFSIGNALED(status))
        {
            printf("Killed %d\n", WTERMSIG(status));
        }
    }

    return 0;
}

