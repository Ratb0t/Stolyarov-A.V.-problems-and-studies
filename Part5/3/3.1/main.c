#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, const char **argv)
{
    if(argc != 4)
    {
        printf("Args count error\n"); return 1;
    }

    FILE *infile, *outfile;
    int str_len_file;

    if(!(infile = fopen(argv[1], "r")))
    {
        printf("Can't open file %s", argv[1]); return 2;
    }

    if (!(outfile = fopen(argv[2], "w")))
    {
        printf("Can't open file %s", argv[2]); return 2;
    }

    if ((str_len_file = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, 0666)) < 0)
    {
        printf("Can't open file %s", argv[3]); return 3;
    }

    int ch;
    int prev_ch = '\n', readed = 0, write_string = 0;

    while ((ch = getc(infile)) != EOF)
    {
        ++readed;
        if(prev_ch == '\n' && ch == ' ')
            write_string = 1;

        if (write_string)
            putc(ch, outfile);
        if (ch == '\n')
        {
            --readed;
            write(str_len_file, &readed, sizeof(readed));
            readed = 0;
            write_string = 0;
        }
        prev_ch = ch;
    }

    close(str_len_file);
    fclose(infile);
    fclose(outfile);

    return 0;
}