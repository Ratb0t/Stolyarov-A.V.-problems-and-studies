#ifndef REINCLUDE_PROTECTION_MY_VECTOR
#define REINCLUDE_PROTECTION_MY_VECTOR

#include "my_types.h"
#include "my_macroses.h"
#include "utilities_functions.h"
#define DEFAULT_CAPACITY 16
#define VECTOR_RESIZE_THRESHOLD 70
#define CAPACITY_FACTOR 1.5

typedef struct my_vector my_vector;

typedef void *my_vector_iterator;


my_vector *my_vector_create(int data_type_size);
/*******************************************************************************/
int my_vector_destroy(my_vector *vec);
/*******************************************************************************/
int my_vector_insert_data(my_vector *vec, unsigned int pos, top_type data);
int my_vector_push_back(my_vector *vec, top_type data);
int my_vector_push_front(my_vector *vec, top_type data);
int my_vector_set_data(my_vector *vec, unsigned int pos, top_type data);
/*******************************************************************************/
my_vector_iterator my_vector_get_first(const my_vector *vec);
my_vector_iterator my_vector_get_end(const my_vector *vec);
my_vector_iterator my_vector_get_item(const my_vector *vec, unsigned int pos);
int my_vector_iter_next(const my_vector *vec, my_vector_iterator *it);
int my_vector_get_data(const my_vector *vec, unsigned int pos, top_type *data);

/*******************************************************************************/
int my_vector_get_size(my_vector *vec);

#endif /*!REINCLUDE_PROTECTION_MY_VECTOR*/