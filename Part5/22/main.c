#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

volatile sig_atomic_t ch;
volatile sig_atomic_t quite;

void handler(int sig)
{
    switch (sig)
    {
    case SIGINT:
        ch = '-';
        if(quite)
            exit(0);
        quite = 1;
        break;
    case SIGQUIT:
        ch = '+';
        break;
    case SIGALRM:        
        quite = 0;
        break;
    default:
        return;
        break;
    }
    write(STDOUT_FILENO, (char *)&ch, 1);
    signal(sig, handler);
    alarm(1);
}

int main()
{
    ch = '+';
    quite = 0;
    
    signal(SIGALRM, handler);
    signal(SIGINT, handler);
    signal(SIGQUIT, handler);

    alarm(1);
    while (1)
    {
        pause();   
    }
    

    return 0;
}