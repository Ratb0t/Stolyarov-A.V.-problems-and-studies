#include "utilities_functions.h"

int my_memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *psrc = (unsigned char *)src;
    unsigned char *pdest = (unsigned char *)dest;

    int step = 8;

    while ((((uintptr_t) psrc & 7) != 0 ||
            ((uintptr_t) pdest & 7) != 0) && n)
    {
        *pdest = *psrc;
        pdest += 1;
        psrc += 1;
        --n;
    }

    for (int i = n / step; i; --i)
    {
        *((unsigned long long *)pdest) = *((unsigned long long *)psrc);
        pdest += step;
        psrc += step;
        n -= step;
    }

    if (n % step)
    {
        for (; n; --n)
        {
            *pdest = *psrc;
            pdest += 1;
            psrc += 1;
        }
    }

    return 1;
}

int my_strlen(const char *buf)
{
    int i;
    for (i = 0; buf[i]; ++i)
    {
    }
    return i;
}