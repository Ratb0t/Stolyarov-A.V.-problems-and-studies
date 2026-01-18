#include <stdio.h>
int get_and_zero(int *val);

int main()
{
    int a = 10;
    int new = 0;
    //printf("%d %d\n", a, new);
    new = get_and_zero(&a);
    //rintf("%d %d\n", a, new);
    return 0;
}

int get_and_zero(int *val)
{
    int old_val = *val;
    *val = 0;
    return old_val;
}