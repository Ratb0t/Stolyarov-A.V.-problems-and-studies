#ifdef DEBUG
#include <stdio.h>
#endif

#include <stdint.h>
#include <stdlib.h>
#include "my_vector.h"

struct my_vector{
    unsigned char *buf;
    unsigned char *end;
    unsigned char *capacity;
    int type_size;
};

static int data_copy(void *vec, const void *data, int type_size);
static int resize_vector(my_vector *vec, unsigned int capacity);
//static int my_vector_get_capacity(my_vector *vec);

int data_copy(void *dest, const void *data, int type_size)
{
    switch (type_size)
    {
    case sizeof(char):
        *(unsigned char *)dest = *(unsigned char *)data;
        break;
    case sizeof(short):
        *(unsigned short *)dest = *(unsigned short *)data;
        break;
    case sizeof(int):
        *(unsigned int *)dest = *(unsigned int *)data;
        break;
    case sizeof(long long):
        *(unsigned long long *)dest = *(unsigned long long *)data;
        break;
    default:
        return my_memcpy(dest, data, type_size);
    }

    return 1;
}

my_vector *my_vector_create(int data_type_size)
{
    my_vector *v;
    if (!(v = malloc(sizeof(my_vector))))
        return NULL;

    if (!(v->buf = malloc(data_type_size * DEFAULT_CAPACITY)))
    {
        free(v);
        return NULL;
    }

    v->capacity = v->buf + data_type_size * DEFAULT_CAPACITY;
    v->type_size = data_type_size;
    v->end = v->buf;

    return v;

}

int my_vector_destroy(my_vector *vec)
{
    free(vec->buf);
    free(vec);
    return 1;
}

int my_vector_get_size(my_vector *vec)
{
    return (vec->end - vec->buf) / vec->type_size;
}

// static int my_vector_get_capacity(my_vector *vec)
// {
//     return (vec->capacity - vec->buf) / vec->type_size;
// }

static int resize_vector(my_vector *vec, unsigned int capacity)
{
    unsigned char *new_buf;
    unsigned int new_capacity = CAPACITY_FACTOR * capacity;
    
    if (!(new_buf = malloc(new_capacity))) return 0;

    my_memcpy(new_buf, vec->buf, vec->end - vec->buf);

    vec->capacity = new_buf + new_capacity;
    vec->end = new_buf + (vec->end - vec->buf);
    free(vec->buf);
    vec->buf = new_buf;

    return 1;
}

int my_vector_push_back(my_vector *vec, top_type data)
{
    int dbg_percent = PERCENT_OF(vec->end - vec->buf, vec->capacity - vec->end);
    if (dbg_percent >= VECTOR_RESIZE_THRESHOLD)
    {
        if (!resize_vector(vec, vec->capacity - vec->buf))
            return 0;
    }
    data_copy(vec->end, &data, vec->type_size);
    vec->end += vec->type_size;

    return 1;
}

int my_vector_push_front(my_vector *vec, top_type data)
{
    return my_vector_insert_data(vec, 0, data);
}

my_vector_iterator my_vector_get_first(const my_vector *vec)
{
    return vec->buf;
}
my_vector_iterator my_vector_get_end(const my_vector *vec)
{
    return vec->end;
}
my_vector_iterator my_vector_get_item(const my_vector *vec, unsigned int pos)
{
    pos *= vec->type_size;
    if(pos >= vec->end - vec->buf)
        return NULL;
    return (vec->buf + pos);
}
int my_vector_get_data(const my_vector *vec, unsigned int pos, top_type *out)
{
    void *data = my_vector_get_item(vec, pos);
    if(!data)
        return 0;
    out->as_ulong = *((unsigned long long *)data);
    return 1;
}

int my_vector_insert_data(my_vector *vec, unsigned int pos, top_type data)
{
    pos *= vec->type_size;
    if(pos >= vec->end - vec->buf)
    {
        if(pos >= vec->capacity - vec->buf)
            if(!resize_vector(vec, pos))return 0;
        
        data_copy(vec->buf + pos, &data, vec->type_size);
        vec->end = vec->buf + pos;
    }
    else
    {
        for (unsigned char *it = vec->end; it > vec->buf + pos; it -= vec->type_size)
        {
            data_copy(it, it - vec->type_size, vec->type_size);
        }
        data_copy(vec->buf + pos, &data, vec->type_size);
    }
    vec->end += vec->type_size;
    return 1;
}

int my_vector_iter_next(const my_vector *vec, my_vector_iterator *it)
{
    *it += vec->type_size;
    return 1;
}

#ifdef MAIN_MY_VECTOR
int main()
{
    my_vector *v = my_vector_create(sizeof(int));
    
    for(int i = 0; i < 20;++i)
    my_vector_push_back(v, (top_type){.as_int = i + 1});
#ifdef DEBUG
    printf("%d\n", my_vector_get_size(v));

    for(int i = 0; i < my_vector_get_size(v);++i)
        printf("%d\n", ((int*)(v->buf))[i]);
#endif
    my_vector_destroy(v);

#ifdef DEBUG
    printf("\n\n\n");
#endif
    my_vector *v2 = my_vector_create(4);
    my_vector_push_back(v2, (top_type){.as_int = 1});
    my_vector_push_back(v2, (top_type){.as_int = 2});
    my_vector_push_back(v2, (top_type){.as_int = 3});
    my_vector_insert_data(v2, 3, (top_type){.as_int = 4});
    my_vector_insert_data(v2, 2, (top_type){.as_int = 5});
    
    my_vector_iterator it = my_vector_get_item(v2, 0);
#ifdef DEBUG
    printf("It - %d\n", *(int*)it);
    for (int i = 0; i < my_vector_get_size(v2); ++i)
        printf("%d\n", ((int *)(v->buf))[i]);
#endif
    // my_vector *v2 = my_vector_create(-1);
    // int buf[] = {1, 2, 3, 4};
    // int buf2[] = {5, 6, 7, 8};
    // my_vector_push_back(v2, (top_type){.as_void = buf});
    // top_type dt;
    // my_vector_get_data(v2, 0, &dt);
#ifdef DEBUG
    // printf("%d\n", ((int*)dt.as_void)[3]);
    // printf("%p\n", my_vector_get_item(v2, my_vector_get_size(v2)));
#endif
    // my_vector_insert_data(v2, 3, (top_type){.as_void = buf});
#ifdef DEBUG
    // printf("%p\n", my_vector_get_item(v2, my_vector_get_size(v2)-1));
    // printf("%d\n", ((int*)dt.as_void)[3]);
#endif
    // my_vector_insert_data(v2, 2, (top_type){.as_void = buf2});
#ifdef DEBUG
    // printf("%p\n", my_vector_get_item(v2, 2));
#endif
    // my_vector_get_data(v2, 2, &dt);
#ifdef DEBUG
    // printf("%d\n", ((int *)dt.as_void)[3]);
#endif

    my_vector_destroy(v2);

    return 0;
}
#endif /*MAIN_MY_VECTOR*/