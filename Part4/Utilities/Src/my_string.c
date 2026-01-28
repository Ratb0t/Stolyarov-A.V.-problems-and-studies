#include "my_string.h"

struct my_string
{
    char *buf;
    int length;
    int capacity;
};

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

static int buf_resize(struct my_string* str)
{
    int new_capacity = str->length * CAPACITY_FACTOR;
    char *new_buf = malloc(new_capacity);
    if(!new_buf)
        return 0;;

    my_memcpy(new_buf, str->buf, str->length);

    return 1;
}

int my_strlen(const char *buf)
{
    int i;
    for(i = 0; buf[i]; ++i){}
    return i;
}
int my_str_get_len(const struct my_string *str)
{
    return str->length;
}

struct my_string *my_str_create(const char *buf)
{
    struct my_string *str = malloc(sizeof(struct my_string));
    if (!str)
        return 0;;

    str->capacity = DEFAULT_CAPACITY_SIZE;
    if(buf)
    {
        str->length = my_strlen(buf);
        if (str->capacity < str->length)
        {
            str->capacity = str->length * CAPACITY_FACTOR;
        }
        str->buf = malloc(str->capacity);
        if (!str->buf)
        {
            free(str);
            return 0;;
        }
        my_str_set_string(str, buf);
    }
    else
    {
        str->buf = malloc(str->capacity);
        str->length = 0;
        if (!str->buf)
        {
            free(str);
            return 0;;
        }
    }

    return str;
}

struct my_string *my_str_create_with_capacity(int capacity)
{
    struct my_string *str = malloc(sizeof(struct my_string));
    if (!str)
        return 0;;
    
    str->capacity = capacity;
    str->buf = malloc(capacity);

    if(!str->buf)
    {
        free(str);
        return 0;;
    }

    return str;
}

int my_str_set_string(struct my_string *str, const char *buf)
{
    if(!buf)
    {
        str->buf[0] = 0;
        str->length = 0;
        return 1;
    }
    int buf_size = my_strlen(buf);
    
    if (str->capacity < buf_size)
    {
        str->capacity = buf_size * CAPACITY_FACTOR;

        char *new_buf = malloc(str->capacity);

        if (!new_buf)
            return 0;

        free(str->buf);
        str->buf = new_buf;
    }

    my_memcpy(str->buf, buf, buf_size);

    return 1;
}

int my_str_destroy(struct my_string *str)
{
    if(str->buf)
        free(str->buf);
    free(str);
    return 1;
}

struct my_string *my_str_insert(const struct my_string *str1, int pos, const struct my_string *str2)
{
    if(str1->length < pos)
        return 0;
        
    struct my_string *result = my_str_create_with_capacity((str1->length + str2->length) * CAPACITY_FACTOR);
    if(!result)
        return 0;;
    
    result->length = str1->length + str2->length;

    //printf("pos %d\n", pos);
    
    int i = 0;
    for (; i < pos; ++i)
    {
        result->buf[i] = str1->buf[i];
    }
    
    //printf("i %d\n", i);
    
    for (int j = 0; str2->buf[j]; ++i, ++j)
    {
        result->buf[i] = str2->buf[j];
    }
    
    //printf("i %d\n", i);

    for(int j = pos+1; j <= str1->length; ++j, ++i)
    {
        result->buf[i] = str1->buf[j];
        //printf("%d %d\t", i, j);
    }
    //printf("\ncreate_my_string_with_capacity %s\n", result->buf);
    return result;
    
}

struct my_string *my_str_concatenation(const struct my_string *str1, const struct my_string *str2)
{
    return my_str_insert(str1, my_str_get_len(str1), str2);
}

const char *my_str_get_data(const struct my_string *str)
{
    return str->buf;
}

int my_str_pushback_char(struct my_string *str, char ch)
{
    if (PERCENT_OF(str->length + 2, str->capacity) >= STRING_RESIZE_THRESHOLD)
        if(!buf_resize(str))
            return 0;
                
    str->buf[str->length] = ch;
    str->length += 1;
    str->buf[str->length + 1] = 0;
    return 1;
}