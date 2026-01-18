#include <stdio.h>
int put_sum(int *a, int *b, int *c);

int main()
{
    int a = 10, b = 20, c = 30;
    int new = 0;
    printf("%d %d %d %d\n", a, b, c, new);
    new = put_sum(&a, &b, &c);
    printf("%d %d %d %d\n", a, b, c, new);
    return 0;
}

int put_sum(int *a, int *b, int *c)
{
    *a += *b + *c;
    *b = *a;
    *c = *a;

    return *a;
}