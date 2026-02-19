#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define RANDOM(lower_range, upper_range) \
    ((lower_range) + (int)(((float)(upper_range) * rand()) / (RAND_MAX + 1.0)))

#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define Min(a, b) (((a) < (b)) ? (a) : (b))

char buf[256];
int generate_binfiles(int count)
{
    srand(time(0));

    for(int i = count; i; --i)
    {
        sprintf(buf, "%d.testdata", i);
        FILE *outfile = fopen(buf, "wb");
        if(!outfile)
            continue;
        for (int i = 0; i < RANDOM(5, 50); ++i )
        {
            int r = RANDOM(1, 10000);
            printf("%s %d\n", buf, r);
            fwrite(&r, sizeof(r), 1, outfile);
        }
        fclose(outfile);
    }

    return 1;
}




int main(int argc, const char **argv)
{
    //generate_binfiles(6);
    
    if(argc <= 1)
    {
        printf("Args count error\n");
        return 1;
    }

    const char *outfile_name = "out.testdata";
    if(argc != 2)
    {
        --argc;
        outfile_name = argv[argc];
    }
        
    FILE *outfile;
    if(!(outfile = fopen(outfile_name, "w")))
    {
        printf("Can't open file %s\n", outfile_name);
        return 2;
    }

    
    for(int i = 1; i < argc; ++i)
    {
        int fd = open(argv[i], O_RDONLY);
        if(fd < 0)
        {
            printf("Can't open file %s\n", argv[i]);
            continue;
        }
        int readed = 0, read_count = 0;
        int min, max;
        int first_read = 1;
        while ((readed = read(fd, buf, sizeof(buf) / sizeof(int)) / sizeof(int)))
        {
            read_count += readed;
            if(first_read)
            {
                /* Инициализация 0 не подходит т.к. 
                числа, например, могут быть только отрицательными 
                в результате max будет содержать 0, которого
                нет во входных данных
                */
                min = ((int *)buf)[0];
                max = ((int *)buf)[0];
                first_read = 0;
            }
            for(int j = 0; j < readed; ++j)
            {
                max = Max(max, ((int*)buf)[j]);
                min = Min(min, ((int *)buf)[j]);
            }
        }
        close(fd);
        fprintf(outfile, "File %s: numbers count - %d; "
            "min - %d; max - %d\n", argv[i], read_count, min, max);
        
    }
    
    

    return 0;
}