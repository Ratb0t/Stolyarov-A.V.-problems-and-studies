#include "my_string.h"

struct my_string
{
    char *buf;
    int length;
    int capacity;
};

int my_strlen(const char *buf)
{
    int i;
    for(i = 0; buf[i]; ++i){}
    return i;
}
int get_my_string_len(struct my_string *str)
{
    return str->length;
}

struct my_string *create_string(const char *buf)
{
    struct my_string *str = malloc(sizeof(struct my_string));
    if (!str)
        return 0;

    str->capacity = 256;
    if(buf)
    {
        str->length = my_strlen(buf);
        if (str->capacity < str->length)
        {
            str->capacity = str->length * CAPACITY_FACTOR;
        }
        str->buf = malloc(str->capacity);
        if (!str->buf)
            return 0;
        set_string(str, buf);
    }
    else
    {
        str->buf = malloc(str->capacity);
        if (!str->buf)
            return 0;
    }

    
    
    
    return str;
}

int set_string(struct my_string *str, const char *buf)
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

        free(str->buf);
        
        str->buf = malloc(str->capacity);
        
        if(!str->buf)
            return 0;
    }

 
    
    for(int i = 0; i <= buf_size; ++i)
    {
        str->buf[i] = buf[i];
    }

    return 1;
}

int destroy_my_string(struct my_string *str)
{
    if(str->buf)
        free(str->buf);
    free(str);
    return 1;
}