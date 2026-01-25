#include <stdio.h>

int number_length(int n)
{
    int i;
    for(i = 0; n; ++i, n /= 10){}

    return i;
}
long long my_pow(int val, int n)
{
    if(!n)
        return 1;
    long long res = val;
    while(n > 1)
    {
        res *= val;
        --n;
    }
    return res;
}

int is_numeric(const char *str);
int main(int argc, const char **argv)
{

    if(argc != 3)
    {
        printf("Need 2 parameters\n");
        return 1;
    }
    for(int i = 1; i < argc; ++i)
    {
        if(!is_numeric(argv[i]))
        {
            printf("Parameters must be numbers only.\n");
            return 2;
        }
    }

    
    int fisrt_symbol_pos;
    int last_symbol_pos;

    long long divizor, res, prom;
    int pos = 0, num = 1, num_legth;

    for (int i = 0; argv[1][i]; ++i)
    {
        if (argv[1][i + 1])
        {
            fisrt_symbol_pos += (argv[1][i] - '0');
            fisrt_symbol_pos *= 10;
        }
        else
            fisrt_symbol_pos += (argv[1][i] - '0');
    }

    for (int i = 0; argv[2][i]; ++i)
    {
        if (argv[2][i + 1])
        {
            last_symbol_pos += (argv[2][i] - '0');
            last_symbol_pos *= 10;
        }
        else
            last_symbol_pos += (argv[2][i] - '0');
    }

    int on_out = last_symbol_pos - fisrt_symbol_pos;
    for (;;)
    {
        res = num * num;
        num_legth = number_length(res);
        divizor = my_pow(10, num_legth - 1);
        pos += num_legth;
        while (pos >= fisrt_symbol_pos && on_out >= 0)
        {
            if (on_out)
                prom = res / divizor;
            else
            {
                divizor = my_pow(10, pos - last_symbol_pos);
                prom = (res / divizor) % 10;
            }

            putchar('0' + (char)(prom % 10));
            --on_out;
            divizor /= 10;
            if (!divizor)
                break;
        }
        if (on_out < 0)
            break;
        ++num;
    }
    putchar('\n');
    return 0;
}

int is_numeric(const char *str)
{
    for (int i = 0; str[i]; ++i)
        if (((unsigned int)(str[i] - '0')) > 9)
            return 0;
    return 1;
}