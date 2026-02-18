#define _LARGEFILE64_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef int fd;

int main(int argc, const char **argv)
{
    if(argc != 2)
    {
        printf("Wrong parameters count\n");
        return 1;
    }

    fd infile;
    if ((infile = open(argv[1], O_RDONLY)) < 0)
    {
        printf("Can't open file %s\n", argv[1]);
        return 2;
    }

    off_t res = lseek64(infile, 0, SEEK_END);
    if (res == (off_t)-1)
    {
        printf("Error: lseek64 can't set pos\n");
        return 3;
    }

    printf("Size file '%s': %ld\n", argv[1], res);
    return 0;
}