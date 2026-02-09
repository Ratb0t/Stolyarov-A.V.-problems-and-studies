#include <stdio.h>
#include <stdlib.h>

#define STDIN_WRITE_TO_FILE
#ifndef STDIN_WRITE_TO_FILE
const char *poem = 
"Humpty Dumpty sat on a wall,\n"
"Humpty Dumpty had a great fall.\n"
"All the king's horses,\n"
"All the king's men,\n"
"Couldn't put Humpty together again.\n";

void write_poem_in_file(char *file_name)
{
    FILE *outfile;
    if(!(outfile = fopen(file_name, "w")))
        return;

    fprintf(outfile, "%s", poem);

    fclose(outfile);
    return;
}

void my_cat(char *file_name)
{

    FILE *infile;
    if (!(infile = fopen(file_name, "r")))
        return;
    
    char *buf = malloc(sizeof(char) * 1024);
    *buf = 0;

    while(fgets(buf, 1024, infile))
        printf(buf);

    fclose(infile);
    free(buf);
    return;
}

void count_strings(char *file_name)
{

    FILE *infile;
    if (!(infile = fopen(file_name, "r")))
        return;

    char *buf = malloc(sizeof(char) * 1024);
    *buf = 0;

    int i = 0;
    while (fgets(buf, 1024, infile))
        ++i;


    printf("String count in file \"%s\": %d\n",file_name, i);
    fclose(infile);
    free(buf);
    return;
}
#endif
#ifdef STDIN_WRITE_TO_FILE
void stdin_to_file(char *file_name)
{
    FILE *outfile;
    if (!(outfile = fopen(file_name, "w")))
        return;
    
    int ch;
    while ((ch = getchar()) != EOF)
    {
        fputc(ch, outfile);
        fflush(outfile);
    }
    
    fclose(outfile);
}
#endif
int main(int argc, char **argv)
{
    if(argc != 2)
        return 1;

#ifndef STDIN_WRITE_TO_FILE
    write_poem_in_file(argv[1]);
    my_cat(argv[1]);
    count_strings(argv[1]);
#else
    stdin_to_file(argv[1]);
#endif
    return 0;
}