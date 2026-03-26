#include <signal.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#define Variant_default

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
        close(pipe_fd[fd_read]);

        dup2(pipe_fd[fd_write], STDOUT_FILENO);
        close(pipe_fd[fd_write]);

        execvp(cmd_line[0], cmd_line);
        perror("exec");
        _exit(1);
    }
    
    return;
}
void read_from_child(int pipe_fd[2]);
#ifdef A
#define WRITE_LINE_COUNT 10
void read_from_child(int pipe_fd[2])
{
    char buf[16];
    int readed;
    int nl_count = WRITE_LINE_COUNT;
    while ((readed = read(pipe_fd[fd_read], buf, sizeof(buf) - 1)) && nl_count)
    {
        int buf_size = readed;
        for (int i = 0; i < buf_size; ++i)
        {
            if(buf[i] == '\n')
            {
                --nl_count;
                if (!nl_count)
                {
                    readed = i + 1;
                    break;
                }
            }
        }
        write(STDOUT_FILENO, buf, readed);
    }
    

    return;    
}
#endif
#ifdef B
/*
    Для проверки "правильности" вывода использовал
    grep '^ ' main.c > grp.testdata
    выбирал только пробелы т.к. vscode заменяет табуляцию пробелами
    ./run.exe cat main.c > out.testdata
    diff grp.testdata out.testdata
*/
void read_from_child(int pipe_fd[2])
{
    FILE *child_out = fdopen(pipe_fd[fd_read], "r");
    
    int nl = 1;
    int in_str = 0;
    int ch;
    while ((ch = getc(child_out)) != EOF)
    {
        if(nl && (ch == ' ' || ch == '\t'))
        {
            in_str = 1;
        }
        if(in_str)
            putchar(ch);
        if (ch == '\n')
        {
            in_str = 0;
            nl = 1;
            continue;
        }

        nl = 0;
    }
    fclose(child_out);

    return;
}
#endif
#ifdef C
void read_from_child(int pipe_fd[2])
{
    FILE *child_out = fdopen(pipe_fd[fd_read], "r");

    int ch;
    int ch_count = 0;
    while ((ch = getc(child_out)) != EOF)
    {
        if(ch_count < 20 && ch != '\n')
            putchar(ch);
        ++ch_count;
        if (ch == '\n')
        {
            putchar(ch);
            ch_count = 0;
        }
        
    }
    fclose(child_out);

    return;
}
#endif
#ifdef D
void read_from_child(int pipe_fd[2])
{
    FILE *child_out = fdopen(pipe_fd[fd_read], "r");

    int ch;
    int first_word = 1;
    int in_string = 0;
    while ((ch = getc(child_out)) != EOF)
    {
        
        if (ch == ' ' || ch == '\t' || ch == '\n')
        {
            if(ch == '\n')
            {
                putchar(ch);
                first_word = 1;
                in_string = 0;
                continue;
            }
            if(in_string)
                first_word = 0;

            in_string = 0;
        }
        else
            in_string = 1;

        if (in_string && first_word)
            putchar(ch);

    }
    fclose(child_out);

    return;
}
#endif


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Not enough parameters\n");
        return 1;
    }

    readers = 0;
    signal(SIGPIPE, handler);

    int pipe_fd[2];
    pipe(pipe_fd);
    

    child_proc(pipe_fd, argv + 1);
    close(pipe_fd[fd_write]);

    read_from_child(pipe_fd);
    close(pipe_fd[fd_read]);

    wait(NULL);

    return 0;
}

