#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

enum
{
    fd_read = 0,
    fd_write
};

struct fd_pipe
{
    int read;
    int write;
};

struct pipes
{
    struct fd_pipe reader;
    struct fd_pipe writer;
};

int close_pipe(struct fd_pipe pp)
{
    close(pp.read);
    close(pp.write);

    return 1;
}

int close_pipes(struct pipes *pps)
{
    close_pipe(pps->reader);
    close_pipe(pps->writer);
    
    return 1;
}

void child_proc_writer(struct pipes *fd, char *cmd_line[])
{
    if (fork() == 0)
    {
        dup2(fd->writer.write, STDOUT_FILENO);
        /* 
            Можно закрыть все дескрипторы т.к. 
            дескриптор стандартного вывода заменен 
            дескриптором трубы
        */
        close_pipes(fd);

        execvp(cmd_line[0], cmd_line);
        perror("exec");
        
        exit(1);
    }

    return;
}

void child_proc_mediator(struct pipes *fd);

#ifdef A
void child_proc_mediator(struct pipes *fd)
{
    if(fork() == 0)
    {
        dup2(fd->writer.read, STDIN_FILENO);
        dup2(fd->reader.write, STDOUT_FILENO);

        close_pipes(fd);

        char ch = 0;
        int nl = 0;
        while ((ch = getchar()) != EOF)
        {
            if(nl)
                putchar(ch);
            if (ch == '\n')
                nl ^= 1;
        }
        /*Необходимо использовать библиотечную функцию exit
        т.к. она сбрасывает буфферы, stdin stdout*/
        exit(0);
    }
    return;
}
#endif
#ifdef B
void child_proc_mediator(struct pipes *fd)
{
    if (fork() == 0)
    {
        dup2(fd->writer.read, STDIN_FILENO);
        dup2(fd->reader.write, STDOUT_FILENO);

        close_pipes(fd);

        char ch = 0;
        int nl = 1;
        int out_str = 0;
        while ((ch = getchar()) != EOF)
        {
            if(nl && (ch == ' ' || ch == '\t'))
            {
                out_str = 1;
            }
            if (out_str)
                putchar(ch);
            if (ch == '\n')
            {
                nl = 1;
                out_str = 0;
                continue;
            }
            nl = 0;
        }
        /*Необходимо использовать библиотечную функцию exit
        т.к. она сбрасывает буфферы, stdin stdout*/
        exit(0);
    }
    return;
}
#endif

#ifdef C
#define SYMBOL_OUT_COUNT 10
void child_proc_mediator(struct pipes *fd)
{
    if (fork() == 0)
    {
        dup2(fd->writer.read, STDIN_FILENO);
        dup2(fd->reader.write, STDOUT_FILENO);

        close_pipes(fd);

        char ch = 0;
        int count = SYMBOL_OUT_COUNT;
        
        while ((ch = getchar()) != EOF)
        {
            if(!count)
            {
                if (ch == '\n')
                {
                    putchar('\n');
                    count = SYMBOL_OUT_COUNT;
                }
                continue;
            }

            putchar(ch);
            --count;
        }
        /*Необходимо использовать библиотечную функцию exit
        т.к. она сбрасывает буфферы, stdin stdout*/
        exit(0);
    }
    return;
}
#endif 
void child_proc_reader(struct pipes *fd, char *cmd_line[])
{
    if (fork() == 0)
    {
        dup2(fd->reader.read, STDIN_FILENO);
        /*
            Можно закрыть все дескрипторы т.к.
            дескриптор стандартного ввода заменен
            дескриптором трубы
        */
        close_pipes(fd);

        execvp(cmd_line[0], cmd_line);
        perror("exec");

        exit(1);
    }

    return;
}

int set_pipes(struct pipes *fds)
{
    int pipe_fd[2];
    pipe(pipe_fd);
    fds->reader.read = pipe_fd[fd_read];
    fds->reader.write = pipe_fd[fd_write];

    pipe(pipe_fd);
    fds->writer.read = pipe_fd[fd_read];
    fds->writer.write = pipe_fd[fd_write];

    return 1;
}


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Not enough parameters\n");
        return 1;
    }

    struct pipes fds;
    set_pipes(&fds);

    int i;
    for (i = 0; argv[i]; ++i)
    {
        if(argv[i][0] == ';' && argv[i][1] == ';')
            break;
    }

    argv[i] = NULL;

    child_proc_writer(&fds, argv + 1);
    child_proc_mediator(&fds);
    child_proc_reader(&fds, argv + i + 1);

    close_pipes(&fds);
    
    while(wait(NULL) > 0)
    {}
    return 0;
}
