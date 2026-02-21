#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 60

typedef struct record
{
    char buf[BUFSIZE];
    int count;
} record;

int str_cmp(const char *str1, const char *str2)
{
    for (int i = 0; str1[i] || str2[i]; ++i)
    {
        if ((str1[i] | 0x20) != (str2[i] | 0x20))
        {
            return 0;
        }
    }
    return 1;
}

int main(int argc, const char *argv[])
{
    /* Лучше было бы использовать бинарное дерево, 
    но реализовывать лень... */
    
    if(argc != 4)
    {
        printf("Args count error\n");
        return 1;
    }


    int infile1 = open(argv[1], O_RDONLY);
    if (infile1 < 0)
    {
        printf("Error: can't open file\n");
        return 2;
    }

    int infile2 = open(argv[2], O_RDONLY);
    if (infile2 < 0)
    {
        printf("Error: can't open file\n");
        return 3;
    }

    int outfile = open(argv[3], O_CREAT | O_EXCL | O_RDWR, 0666);
    if (outfile < 0)
    {
        printf("Error: can't open file\n");
        return 3;
    }
    record r1, r2;
    int readed_r1, readed_r2;

    while ((readed_r2 = read(infile2, &r2, sizeof(record))))
    {
        write(outfile, &r2, readed_r2);
    }

    lseek(outfile, 0, SEEK_SET);
    close(infile2);

    while((readed_r1 = read(infile1, &r1, sizeof(record))))
    {
        while ((readed_r2 = read(outfile, &r2, sizeof(record))))
        {
            if (str_cmp(r1.buf, r2.buf))
            {
                r1.count += r2.count;
                lseek(outfile, -readed_r2, SEEK_CUR);
                write(outfile, &r1, sizeof(record));
            }
        }

        lseek(outfile, 0, SEEK_SET);
    }

    close(outfile);
    close(infile1);
    return 0;
}