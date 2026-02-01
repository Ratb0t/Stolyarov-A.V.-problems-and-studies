#ifndef REINCLUDE_PROTECTION_MY_VECTOR
#define REINCLUDE_PROTECTION_MY_VECTOR

#include "my_types.h"
#include "utilities_functions.h"
#define DEFAULT_CAPACITY 16

typedef struct my_vector my_vector;

typedef top_type *my_vector_iterator;


my_vector *my_vector_create(int data_type_size);
/*******************************************************************************/
int my_vector_destroy(my_vector *vec);
/*******************************************************************************/
int my_vector_push_back(my_vector *vec, top_type data);
int my_vector_insert_data(my_vector *vec, unsigned int pos, top_type data);
int my_vector_set_data(my_vector *vec, unsigned int pos, top_type data);
/*******************************************************************************/
my_vector_iterator *my_vector_get_first(my_vector *vec);
my_vector_iterator *my_vector_get_item(my_vector *vec, unsigned int pos);
my_vector_iterator *my_vector_get_last(my_vector *vec);
/*******************************************************************************/
int my_vector_get_size(my_vector *vec);

#endif /*!REINCLUDE_PROTECTION_MY_VECTOR*/