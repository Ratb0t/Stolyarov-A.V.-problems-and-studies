#include <stdio.h>

int strip(char *str)
{
    char *iter = str;
    for(int i = 0; iter[i]; ++i)
    {
        if(str[i] == ' ')
        {
            iter += 1;
            while(*iter != ' ')
                iter += 1;
            str[i] = *iter;
        }
    }
    return 1;
}

int main()
{
    char *str = "Delete All Spaces";
    strip(str);
    printf("%s", str);
    return 0;
}