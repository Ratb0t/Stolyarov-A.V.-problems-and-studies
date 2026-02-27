#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

typedef char **string_array;

int format_cmd_line(int argc, char *argv[], string_array *cmd_line, int *cmd_line_size)
{
    int i;
    for(i = 0; i < argc - 1; ++i)
    {
        (*cmd_line)[i] = argv[i + 1];
    }
    (*cmd_line)[i] = NULL;
    return i;
}



int main(int argc, char *argv[])
{

    if(argc < 2)
    {
        printf("Args count error\n");
        //return 1;
    }

    int status;
    int pid;
    int cmd_line_size = argc;
    /*
    cmd_line является указателем на массив указателей, почему так?
    Потому что в Си не возможно присвоить массив другому массиву, поэтому
    приходится выкручиваться. Поэтому в format_cmd_line необходимо
    принимать указатель на указатель char_poiter_arr чтобы изменить внешний указатель
    cmd_line
    */
    string_array cmd_line = malloc(cmd_line_size * sizeof(char *));
    format_cmd_line(argc, argv, &cmd_line, &cmd_line_size);

    pid = fork();
    if(pid < 0)
    {
        perror("Fork");
        return 1;
    }
    if(pid == 0)
    {
  
        execvp(cmd_line[0], cmd_line);
        perror(cmd_line[0]);
        exit(1);
    }
    else
    {
        wait(&status);
  
        if (WIFEXITED(status) && !WIFSIGNALED(status))
        {
            printf("exited\n");
        }
        if (WIFSIGNALED(status))
        {
            printf("Killed %d\n", WTERMSIG(status));
        }
    }
    
    free(cmd_line);

    return 0;
}