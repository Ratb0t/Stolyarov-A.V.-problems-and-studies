#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

volatile sig_atomic_t sig_on_print;
volatile sig_atomic_t sigint_count;

void handler(int sig)
{
    switch (sig)
    {
    case SIGINT:
        signal(SIGINT, handler);
        ++sigint_count;
        break;
    case SIGUSR1:
        signal(SIGUSR1, handler);
        sig_on_print = 1;
        break;
    default:
        break;
    }
    return;
}

int main()
{
    printf("pid %d\n", getpid());
    signal(SIGINT, handler);
    signal(SIGUSR1, handler);

    sig_on_print = 0;

    while(1)
    {
        pause();
        if (sig_on_print)
        {
            printf("\n%d\n", sigint_count);
            sig_on_print = 0;
        }

    }

    return 0;
}