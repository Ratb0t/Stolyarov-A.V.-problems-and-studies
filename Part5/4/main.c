#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFSIZE 4096

char buf[BUFSIZE];

int main(int argc, const char *argv[])
{
    if(argc != 2)
    {
        printf("Args count error\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd < 0)
    {
        printf("Can't open file %s", argv[1]);
        return 2;
    }

    int readed = 0;
    int string_count = 0;
    while((readed = read(fd, buf, sizeof(buf))))
    {
        for(int i = 0; i < readed; ++i)
        {
            if(buf[i] == '\n')
                ++string_count;
        }
    }

    printf("%d\n", string_count);

    close(fd);

    return 0;
}