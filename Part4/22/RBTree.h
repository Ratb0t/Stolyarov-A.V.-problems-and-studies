#ifndef RBTREE_REINCLUDE_SENTRY
#define RBTREE_REINCLUDE_SENTRY

#define REMOVE_WITH_REPLACE_NODE
#define USE_TRANSPLANT_FUNCTION

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
enum compare_function_value{
    compare_less = -1,
    compare_equal,
    compare_great
};

typedef int (*compare_function)(void *a, void *b);

typedef struct node
{
    int key;
    Color color;
    struct node *parent;
    struct node *left;
    struct node *right;
} node;

typedef struct RBTree
{
    node *root;
    node *nil;
    compare_function function;
} RBTree;

node *create_node(RBTree *tree, int key);
node *create_nil();
RBTree *create_tree(compare_function function);

node *RBTree_find_node(RBTree *tree, int key);

void destroy_tree(RBTree *tree);

void left_rotate(RBTree *tree, node *rt);
void right_rotate(RBTree *tree, node *rt);
int get_granson_type(RBTree *tree, node *n);
void swap_color(node *n1, node *n2);

int RB_fixup(RBTree *tree, node *n);
void insert_fixup(RBTree *tree, node *n);
void descending_fixup(RBTree *tree, node *n);

int insert(RBTree *tree, int key);

void RBTree_remove_node(RBTree *tree, node *x);

/*tests*/
int checkBlackHeight(RBTree *tree, node *current, int *valid);
int isRBTreeBalanced(RBTree *tree);
;

#endif /*!RBTREE_REINCLUDE_SENTRY*/