#ifndef MY_RBTREE_REINCLUDE_SENTRY
#define MY_RBTREE_REINCLUDE_SENTRY

#define REMOVE_WITH_REPLACE_NODE
#define USE_TRANSPLANT_FUNCTION
#include "my_types.h"

enum compare_function_value{
    compare_less = -1,
    compare_equal,
    compare_great
};

typedef int (*compare_function)(top_type a, top_type b);
typedef int (*accept_to_data)(top_type data);


typedef struct node 
{
    top_type data;
}
node;

typedef struct RBTree RBTree;

RBTree *RBTree_create(compare_function function);

node *RBTree_find(RBTree *tree, top_type data);

void RBTree_clear(RBTree *tree, accept_to_data data_function);
void RBTree_destroy(RBTree *tree, accept_to_data data_function);

int RBTree_insert(RBTree *tree, top_type data);

void RBTree_remove_node(RBTree *tree, node *remove);
void RBTree_walk(RBTree *tree, accept_to_data data_function);
int RBTree_is_empty(RBTree *tree);

#endif /*!MY_RBTREE_REINCLUDE_SENTRY*/