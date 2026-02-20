#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 4096

unsigned char buf[BUFSIZE];

int xor_file(const char *filename, unsigned int key)
{
    int iofile = open(filename, O_RDWR);
    if(iofile < 0) return 0;

    int readed, reminder = 0;
    while ((readed = read(iofile, buf, BUFSIZE)))
    {
        reminder = readed % sizeof(int);
        
        int i = 0;
        for (; i < (readed - reminder) / sizeof(int); ++i)
        {
            ((int*)buf)[i] ^= key;
        }
        if(reminder)
        {
            key = key >> 8 * (sizeof(int) - reminder);
            ((int *)buf)[i] ^= key;
        }
        lseek(iofile, -readed, SEEK_CUR);
        write(iofile, buf, readed);
    }
    

    close(iofile);
    return 1;
}

int main(int argc, const char *argv[])
{
    if(argc != 3)
    {
        printf("Wrong args count\n1. File name\n2. Key\n");
        return 1;
    }

    xor_file(argv[1], (unsigned int)atoi(argv[2]));

    return 0;
}