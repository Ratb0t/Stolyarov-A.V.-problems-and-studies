#include <stdint.h>
#include <stdlib.h>
#include "my_vector.h"

struct my_vector{
    unsigned char *buf;
    unsigned char *end;
    unsigned char *capacity;
    int type_size;
};


int data_copy(my_vector *vec, void *data)
{
    switch (vec->type_size)
    {
    case -1:
        *(void**) vec = data;
        break;
    case sizeof(int):
        *(unsigned int*)vec = *(unsigned int*)data;
        break;
    case sizeof(long long):
        *(unsigned long long*)vec = *(unsigned long long*)data;
        break;
    default:
        return my_memcpy(vec, data, vec->type_size);
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

    v->capacity = DEFAULT_CAPACITY;
    v->type_size = data_type_size;
    v->end = v->buf;

    return v;

}