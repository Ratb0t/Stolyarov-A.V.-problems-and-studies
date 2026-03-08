#ifndef RBTREE_REINCLUDE_SENTRY
#define RBTREE_REINCLUDE_SENTRY

#define REMOVE_WITH_REPLACE_NODE
#define USE_TRANSPLANT_FUNCTION
#include "my_types.h"
typedef enum Color
{
    BLACK,
    RED
} Color;
enum Grandson_types
{
    IT_ROOT,
    OUTSIDE_LEFT,
    INSIDE_LEFT,
    OUTSIDE_RIGHT,
    INSIDE_RIGHT
};

typedef int (*compare_function)(top_type a, top_type b);
typedef int (*accept_to_data)(top_type data);


typedef struct node
{
    top_type data;
    Color color;
    struct node *parent;
    struct node *left;
    struct node *right;
} node;

typedef struct RBTree
{
    node *root;
    node *nil;
    compare_function cmp;
} RBTree;

node *create_node(RBTree *tree, top_type data);
node *create_nil();
RBTree *RBTree_create(compare_function function);

node *RBTree_find(RBTree *tree, top_type data);

void RBTree_clear(RBTree *tree, accept_to_data data_function);
void RBTree_destroy(RBTree *tree, accept_to_data data_function);


void left_rotate(RBTree *tree, node *rt);
void right_rotate(RBTree *tree, node *rt);
int get_granson_type(RBTree *tree, node *n);
void swap_color(node *n1, node *n2);

int RB_fixup(RBTree *tree, node *n);
void insert_fixup(RBTree *tree, node *n);
void descending_fixup(RBTree *tree, node *n);

int RBTree_insert(RBTree *tree, top_type data);

void RBTree_remove_node(RBTree *tree, node *x);
void RBTree_walk(RBTree *tree, accept_to_data data_function);

int RBTree_is_empty(RBTree *tree);
/*tests*/
int checkBlackHeight(RBTree *tree, node *current, int *valid);
int isRBTreeBalanced(RBTree *tree);

#endif /*!RBTREE_REINCLUDE_SENTRY*/