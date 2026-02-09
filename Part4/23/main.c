#include <stdio.h>

const char *poem = 
"Humpty Dumpty sat on a wall,\n"
"Humpty Dumpty had a great fall.\n"
"All the king's horses,\n"
"All the king's men,\n"
"Couldn't put Humpty together again.\n";



int main(int argc, char **argv)
{
    if(argc != 2)
        return 1;
    
    FILE *outfile = fopen(argv[1], "w");

    fprintf(outfile, "%s", poem);

    return 0;
}