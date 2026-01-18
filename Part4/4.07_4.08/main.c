#include <stdio.h>
int spaces_count(const char *str);
int space_count_rv(const char* str);
int spaces_count_recursive(const char *str, int count);
int main()
{

    int new = space_count_rv("Hi man, hi ");
    printf("%d\n", new);
    return 0;
}

int spaces_count(const char *str)
{
    int count = 0;
    for(; *str; ++str)
        if(*str == ' ')
            ++count;

    return count;
}

/*Недостаток рекурсивного решения всегда будет 
накладным вызовом функции и ограниченного количества
таких вызовов из-за возможности переполнения стека*/
int spaces_count_recursive(const char *str, int count)
{
    if(!*str)
        return count;
    if(*str == ' ')
        ++count;
    return spaces_count_recursive(str + 1, count);
}
int space_count_rv(const char *str)
{
    return spaces_count_recursive(str, 0);
}