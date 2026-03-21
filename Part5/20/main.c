#include <signal.h>
#include <unistd.h>
#include <errno.h>

const char quite_msg[] = "Press Ctrl-C to quite\n";
const char bye_msg[] = "\nGood bye\n";

volatile sig_atomic_t sig_cought;

void handler(int sig)
{
    sig_cought = 1;
    return;
}

int main()
{
    signal(SIGINT, handler);
    sig_cought = 0;
    write(STDOUT_FILENO, quite_msg, sizeof(quite_msg) - 1);
    

    /* Позволит добавить в обработчик дополнительные сигналы
    для обработки */
    while(!sig_cought)
        pause();

    write(STDOUT_FILENO, bye_msg, sizeof(bye_msg));
    return 0;
}