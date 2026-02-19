#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 4096

#define Min(a, b) (((a) < (b)) ? (a) : (b))

char buf[BUFSIZE];

void set_buf(char *dest, unsigned char val, int size)
{
    for(int i = 0; i < size; ++i)
    {
        dest[i] = val;
    }

    return;
}

int hex_to_int(char ch, int *result)
{
    if ((ch | 0x20) >= 'a' && (ch | 0x20) <= 'f')
    {
        *result = (ch | 0x20) - 'a' + 10;
        return 1;
    }
    if(ch >= '0' && ch <= '9')
    {
        *result = ch - '0';
        return 1;
    }
    return 0;
}

int dec_to_int(char ch, int *result)
{
    if (ch >= '0' && ch <= '9')
    {
        *result = ch - '0';
        return 1;
    }

    return 0;
}

int get_byte_value(const char *b, int *result)
{
    int len = 0;
    *result = 0;
    int rs;
    while (b[len])
    {
        ++len;
    }
    if(len == 0) 
    {
        *result = -1;
        return 0;
    }

    if (len > 2 && b[0] == '0' && b[1] == 'x')
    {
        for(int i = 2; b[i]; ++i)
        {
            *result *= 16;
            if(!hex_to_int(b[i], &rs))
            {
                return 0;
            }
            *result += rs;
        }
        return 1;
    }

    for(int i = 0; b[i]; ++i)
    {
        *result *= 10;
        if(!dec_to_int(b[i], &rs))
        {
            return 0;
        }
        *result += rs;
    }
    
    return 1;
}



/*
argv
1. Имя файла
2. Начальная позиция
3. Длина
4. Значение байта
*/
int main(int argc, const char *argv[])
{

    if (argc != 5)
    {
        printf("Args count error\n");
        return 1;
    }
    
    int byte, length, start_pos;
    if (!get_byte_value(argv[4], &byte))
    {
        if (byte == -1)
        {
            printf("The character representation of a byte is empty\n");
            return 1;
        }
        printf("An unknown character was encountered in the symbolic representation of a byte.\n");
        return 2;
    }
    if(byte > 255)
    {
        printf("The character representation of the byte\n"
            "is out of range for the value (hex 0xFF or decimal 255).\n"
            "Current value 0x%X/%d\n", byte, byte);
        return 3;
    }
    
    int outfile = open(argv[1], O_WRONLY);
    if(outfile < 0)
    {
        printf("Can't open file %s\n", argv[1]);
        return 4;
    }
    start_pos = atoi(argv[2]);
    length = atoi(argv[3]);
    
    lseek(outfile, start_pos, SEEK_SET);
    
    set_buf(buf, (unsigned char)byte, Min(length, BUFSIZE));

    while (length > 0)
    {
        write(outfile, buf, Min(length, BUFSIZE));
        length -= BUFSIZE;
    }

    close(outfile);
    return 0;
}