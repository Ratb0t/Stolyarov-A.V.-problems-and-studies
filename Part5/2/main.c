#include <unistd.h>

#define BUFSIZE 1024

char buf[BUFSIZE];
const char str_err[] = "Read error\n";
int main()
{
    int readed;
    while ((readed = read(STDIN_FILENO, buf, BUFSIZE)))
    {
        if(readed < 0)
        {
            write(STDOUT_FILENO, str_err, sizeof(str_err));
            return 1;
        }

        write(STDOUT_FILENO, buf, readed);
    }
    
    return 0;
}