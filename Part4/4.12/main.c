#include <stdio.h>

int str_len(const char *str);
const char *BMH(const char *str, const char *sub_str);

int find_sub_str_in_str(int argc, const char **args);

int find_sub_str_in_str_count(int argc, const char **args);

int main(int argc, const char **argv)
{

    if(argc < 3)
        {printf("Not enough arguments passed!"); return 1;}

    find_sub_str_in_str(argc, argv);
    
    printf("\n\n");
    
    find_sub_str_in_str_count(argc, argv);
    return 0;
}
int str_len(const char *str)
{
    int i = 0;
    for (i = 0; str[i]; ++i){}
    return i;
}

const char *BMH(const char *str, const char *sub_str)
{
    int size_str = str_len(str);
    int size_sub_str = str_len(sub_str);

    char symbols[256];
    for (int i = 0; i < 256; ++i)
        symbols[i] = size_sub_str;

    for (int i = size_sub_str - 2; i >= 0; --i)
    {
        symbols[sub_str[i]] = size_sub_str - i - 1;
        // printf("%c - %d\n", sub_str[i], symbols[sub_str[i]]);
    }

    int current_pos = 0;

    while (current_pos <= size_str - size_sub_str)
    {
        int sub_pos = size_sub_str - 1;
        while (sub_pos >= 0 && sub_str[sub_pos] == str[current_pos + sub_pos])
            --sub_pos;

        if (sub_pos < 0)
        {
            return str + current_pos;
            break;
        }
        else
        {
            int offset = str[current_pos + sub_pos];
            current_pos += symbols[offset];
            // printf("%s\n", str + current_pos);
        }
    }

    return 0;
}

int find_sub_str_in_str(int argc, const char **args)
{
    const char *sub_str = args[1];
    for(int i = 2; i < argc; ++i)
    {
        if (BMH(args[i], sub_str))
            printf("%s\n", args[i]);
    }

    return 1;
}

int find_sub_str_in_str_count(int argc, const char **args)
{
    const char *sub_str = args[1];
    const char *arg = 0;
    int count = 0;
    for (int i = 2; i < argc; ++i)
    {
        arg = args[i];
        while ((arg = BMH(arg, sub_str)) && str_len(arg))
        {
            ++count;
            arg += str_len(sub_str);
        }
        if (count)
        {
            printf("%s\nNumber of occurrences \"%s\": %d\n", args[i], sub_str, count);
            count = 0;
        }
    }

    return 1;
}