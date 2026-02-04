#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...)
#endif /*DEBUG*/

#define START_CAPACITY 2
#define CAPACITY_FACTOR 2

typedef struct stackdbl
{
    double *buf;
    double *head;
    double *capacity;
} stackdbl;

stackdbl *stackdbl_init();
void stackdbl_destroy(stackdbl *st);
int stackdbl_push(stackdbl *st, double data);
double stackdbl_pop(stackdbl *st);
int stackdbl_empty(stackdbl *st);

int main()
{
    stackdbl *st = stackdbl_init();
    
    for(int i = 0; i < 20; ++i)
    {
        stackdbl_push(st, i+1);
    }

    /*Если DEBUG не определе, то stackdbl_empty не вызывается*/
    DEBUG_PRINT("Stac not empty %d\n", stackdbl_empty(st));

    for (int i = 0; i < 20; ++i)
    {
        /*Если DEBUG не определе, то stackdbl_pop не вызывается*/
        DEBUG_PRINT("%lf\n", stackdbl_pop(st));
    }

    stackdbl_destroy(st);
    return 0;
}

stackdbl *stackdbl_init()
{
    stackdbl *item;
    if(!(item = malloc(sizeof(stackdbl))))
        return NULL;
    if (!(item->buf = malloc(sizeof(double) * START_CAPACITY)))
    {
        free(item);
        return NULL;
    }

    item->head = item->buf;
    item->capacity = item->buf + START_CAPACITY;

    return item;
}

void stackdbl_destroy(stackdbl *st)
{
    free(st->buf);
    free(st);

    return;
}

int stackdbl_empty(stackdbl *st)
{
    return !!(st->head - st->buf);
}

int stackdbl_push(stackdbl *st, double data)
{
    if(st->head == st->capacity)
    {
        double *new_buf;
        int new_capacity = (st->capacity - st->buf);
        new_capacity *= CAPACITY_FACTOR ;
        if (!(new_buf = malloc(new_capacity * sizeof(double))))
            return 0;
        
        for(int i = 0; i < st->capacity - st->buf; ++i)
        {
            new_buf[i] = st->buf[i];
        }
        st->head = new_buf + (st->capacity - st->buf);
        st->capacity = new_buf + new_capacity;
        free(st->buf);
        st->buf = new_buf;
    }

    *st->head = data;
    st->head += 1;

    return 1;
}

double stackdbl_pop(stackdbl *st)
{
    if(!stackdbl_empty(st))
        return 0;
    
    st->head -= 1;
    return *st->head;
}