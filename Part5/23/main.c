#define _POSIX_C_SOURCE 199309L
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define WAIT_TIMER 5

volatile sig_atomic_t quite;
volatile sig_atomic_t out_stat;
volatile sig_atomic_t activity_flag;
volatile sig_atomic_t ask_user;

const char question[] = "\nAre you here, mate?\n";

void handler_sigint(int sig)
{
    int save_errno = errno;
    signal(SIGINT, handler_sigint);
    if (quite)
    {
        write(STDOUT_FILENO, "\n", 1);
        _exit(0);
    }
    quite = 1;
    out_stat = 1;
    ask_user = 1;
    alarm(WAIT_TIMER);
    
    errno = save_errno;
    return;  
}

void handler_sigalarm(int sig)
{
    int save_errno = errno;
    signal(sig, handler_sigalarm);

    if (!activity_flag && ask_user)
    {
        write(STDOUT_FILENO, question, sizeof(question)-1);
        ask_user = 0;
    }
    quite = 0;
    activity_flag = 0;
    alarm(WAIT_TIMER);
    
    errno = save_errno;
}


int main()
{
    ask_user = 1;
    activity_flag = 0;
    out_stat = 0;
    quite = 0;

    signal(SIGINT, handler_sigint);
    signal(SIGALRM, handler_sigalarm);

    int ch = 0;
    int line_count = 0;
    int char_count = 0;
    alarm(WAIT_TIMER);
    while (1)
    {
        if (out_stat)
        {
            out_stat = 0;
            printf("\nLines count: %d\nSymbols count: %d\n", line_count, char_count);
            continue;
        }
        if (read(STDIN_FILENO, &ch, 1) == -1 && errno == EINTR)
        {
            errno = 0;
            continue;
        }
        if(ch == '\n')
            ++line_count;
        else
            ++char_count;
        
        activity_flag = 1;

    }
    
    return 0;
}