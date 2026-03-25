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

void child_proc(int pipe_fd[2])
{
    const char str[] = "Hello world. \nI'm try to use pipe, \nfor send this message to my parrent\n";
    close(pipe_fd[fd_read]);
    write(pipe_fd[fd_write], str, sizeof(str) - 1);
    close(pipe_fd[fd_write]);
    _exit(0);
}

int main()
{
    int pipe_fd[2];
    pipe(pipe_fd);
    int pid = fork();
    if (pid == 0)
    {
        child_proc(pipe_fd);
    }
    if (pid == -1)
    {
        perror("Fork");
        return 1;
    }

    close(pipe_fd[fd_write]);

    char buf[16];
    int readed = 0;
    while ((readed = read(pipe_fd[fd_read], buf, 16)))
    {
        write(STDOUT_FILENO, buf, readed);
    }
    wait(NULL);
    close(pipe_fd[fd_read]);
    return 0;
}
