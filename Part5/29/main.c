#include <signal.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

sig_atomic_t readers;
struct pipe_fd{
    int read;
    int write;
};
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

void child_proc(struct pipe_fd to_child, struct pipe_fd from_child, char *cmd_line[])
{
    ++readers;
    if (fork() == 0)
    {
        /*printf("kill -s SIGINT %d\n", getpid());
        printf("kill -s SIGTERM %d\n", getpid());
        printf("kill -s SIGKILL %d\n", getpid());*/
        close(to_child.write);
        close(from_child.read);

        dup2(to_child.read, STDIN_FILENO);
        close(to_child.read);

        dup2(from_child.write, STDOUT_FILENO);
        close(from_child.write);

        execvp(cmd_line[0], cmd_line);
        perror("exec");
        _exit(1);
    }
    
    return;
}

void write_to_child(struct pipe_fd *to_child)
{
    char buf[16];
    int written;
    for (int i = 1; i <= 1000000 && readers; ++i)
    {
        written = sprintf(buf, "%d\n", i);
        write(to_child->write, buf, written);
    }
    
    return;    
}

int count_newlines(const char *buf, int size)
{
    int nlc = 0;
    for (int i = 0; i < size; ++i)
    {
        if(buf[i] == '\n')
            ++nlc;
    }
    return nlc;
}

int read_from_child(struct pipe_fd *from_child)
{
    char buf[1024];
    int readed = 0;
    int accum = 0, nl_count = 0;
    while ((readed = read(from_child->read, buf, 1024)))
    {
        accum += readed;
        nl_count += count_newlines(buf, readed);
    }
    
    printf("Symbols count: %d\nLines count: %d\n", accum, nl_count);
    return accum;
}

int analyz_child_out(struct pipe_fd *from_child, struct pipe_fd *to_child)
{
    int pid = fork();
    if(pid == 0)
    {
        close(to_child->write);
        read_from_child(from_child);
        exit(0);
    }
    return pid;
}


void set_pipes(struct pipe_fd *to_child, struct pipe_fd *from_child)
{
    int pipe_fd[2];

    pipe(pipe_fd);
    to_child->write = pipe_fd[fd_write];
    to_child->read = pipe_fd[fd_read];

    pipe(pipe_fd);
    from_child->write = pipe_fd[fd_write];
    from_child->read = pipe_fd[fd_read];

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

    struct pipe_fd to_child, from_child;
    
    set_pipes(&to_child, &from_child);

    child_proc(to_child, from_child, argv + 1);
    close(to_child.read);
    close(from_child.write);

    int alzr_pid = analyz_child_out(&from_child, &to_child);
    close(from_child.read);

    write_to_child(&to_child);
    close(to_child.write);

    int pid = wait(&status);
    if(pid != alzr_pid)
        pid = wait(NULL);
    else
        pid = wait(&status);
    
    if(pid > 0)
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
