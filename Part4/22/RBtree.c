#include "RBTree.h"
#include <stdlib.h>

typedef void (*accept_to_node)(node *n);

node *RBTree_find_node(RBTree *tree, top_type data)
{
    node *n = tree->root;
    while (n != tree->nil)
    {
        switch (tree->cmp(n->data, data))
        {
        case compare_equal:
            return n;
            break;
        case compare_great:
            n = n->left;
            break;
        case compare_less:
            n = n->right;
        default:
            break;
        }
        // if (n->key == key)
        //     return n;
        // else if (n->key > key)
        //     n = n->left;
        // else
        //     n = n->right;
    }
    if(n == tree->nil)
        n = NULL;
    return n;
}

node *minimal_node(RBTree *tree, node *n, int *length)
{
    int i = 0;
    node *p = n;
    while (n != tree->nil)
    {
        p = n;
        n = n->left;
        ++i;
    }

    if(length)
        *length = i;

    return p;
}
node *maximum_node(RBTree *tree, node *n, int *length)
{
    int i = 0;
    node *p = n;
    while (n != tree->nil)
    {
        p = n;
        n = n->right;
        ++i;
    }
    
    if (length)
        *length = i;

    return p;
}

int RBTree_insert(RBTree *tree, top_type data)
{
    node *cur = tree->root;
    node *cur_parent = tree->nil;
    while (cur != tree->nil)
    {
        cur_parent = cur;
        if (tree->cmp(cur->data, data) == compare_equal)
        {
            tree->root->color = BLACK;
            return 0;
        }

        descending_fixup(tree, cur);

        if (tree->cmp(cur->data, data) == compare_great)
            cur = cur->left;
        else
            cur = cur->right;
        
    }
    node *new_node = create_node(tree, data);
    new_node->parent = cur_parent;
    if (cur_parent == tree->nil)
        tree->root = new_node;
    else if (tree->cmp(cur_parent->data, data) == compare_great)
        cur_parent->left = new_node;
    else
        cur_parent->right = new_node;

    insert_fixup(tree, new_node);

    tree->root->color = BLACK;
    return 1;
}

node *create_node(RBTree *tree, top_type data)
{
    node *new = malloc(sizeof(node));
    if (!new)
    {
        return NULL;
    }
    new->color = RED;
    new->data = data;
    new->left = tree->nil;
    new->right = tree->nil;
    new->parent = tree->nil;

    return new;
}

node *create_nil()
{
    node *new = malloc(sizeof(node));
    if (!new)
    {
        return NULL;
    }
    new->color = BLACK;
    new->data.as_void = NULL;
    new->left = new;
    new->right = new;
    new->parent = new;

    return new;
}

RBTree *create_tree(compare_function function)
{
    node *nil = create_nil();
    if (nil == NULL)
        return NULL;

    RBTree *tree = malloc(sizeof(RBTree));
    if (tree == NULL)
    {
        free(nil);
        return NULL;
    }
    tree->nil = nil;
    tree->root = nil;
    tree->cmp = function;
    return tree;
}

void destroy_tree(RBTree *tree)
{
    free(tree->nil);
    free(tree);
    return;
}

void left_rotate(RBTree *tree, node *rt)
{
    if (rt == tree->nil)
        return;

    node *r_son = rt->right;
    rt->right = r_son->left;
    if (r_son->left != tree->nil)
        r_son->left->parent = rt;

    r_son->parent = rt->parent;
    if (rt->parent == tree->nil)
        tree->root = r_son;
    else if (rt->parent->left == rt)
        rt->parent->left = r_son;
    else
        rt->parent->right = r_son;
    r_son->left = rt;
    rt->parent = r_son;

    return;
}

void right_rotate(RBTree *tree, node *rt)
{
    if (rt == tree->nil)
        return;

    node *l_son = rt->left;
    rt->left = l_son->right;
    if (l_son->right != tree->nil)
        l_son->right->parent = rt;

    l_son->parent = rt->parent;
    if (rt->parent == tree->nil)
        tree->root = l_son;
    else if (rt->parent->right == rt)
        rt->parent->right = l_son;
    else
        rt->parent->left = l_son;
    l_son->right = rt;
    rt->parent = l_son;

    return;
}

int get_granson_type(RBTree *tree, node *n)
{
    if (n->parent == tree->nil)
        return IT_ROOT;
    if (n->parent->parent->left == n->parent)
    {
        if (n->parent->left == n)
            return OUTSIDE_LEFT;

        return INSIDE_LEFT;
    }

    if (n->parent->right == n)
        return OUTSIDE_RIGHT;

    return INSIDE_RIGHT;
}

void swap_color(node *n1, node *n2)
{
    Color tmp = n1->color;
    n1->color = n2->color;
    n2->color = tmp;
    return;
}

int RB_fixup(RBTree *tree, node *n)
{
    switch (get_granson_type(tree, n))
    {
    case OUTSIDE_LEFT:
        swap_color(n->parent, n->parent->parent);
        right_rotate(tree, n->parent->parent);
        break;
    case INSIDE_LEFT:
        swap_color(n, n->parent->parent);
        left_rotate(tree, n->parent);
        right_rotate(tree, n->parent);
        break;
    case INSIDE_RIGHT:
        swap_color(n, n->parent->parent);
        right_rotate(tree, n->parent);
        left_rotate(tree, n->parent);
        break;
    case OUTSIDE_RIGHT:
        swap_color(n->parent, n->parent->parent);
        left_rotate(tree, n->parent->parent);
    default:
        return 0;
        break;
    }
    if (n->parent == tree->nil)
        tree->root = n;
    return 1;
}

void insert_fixup(RBTree *tree, node *n)
{
    if (n->parent->color == BLACK)
        return;

    RB_fixup(tree, n);

    return;
}

void descending_fixup(RBTree *tree, node *n)
{
    if (n->left->color == RED && n->right->color == RED)
    {
        n->color = RED;
        n->left->color = BLACK;
        n->right->color = BLACK;
    }
    if (n->color == RED && n->parent->color == RED)
    {
        RB_fixup(tree, n);
    }
    return;
}

void RBTransplant(RBTree *tree, node *x, node *v)
{
    if (x->parent == tree->nil)
        tree->root = v;
    else if (x == x->parent->left)
        x->parent->left = v;
    else
        x->parent->right = v;

    v->parent = x->parent;
    return;
}

void delete_fixup(RBTree *tree, node *x)
{
    node *w;
    while (x != tree->root && x->color == BLACK)
    {
        if(x == x->parent->left)
        {
            w = x->parent->right;
            if(w->color == RED)
            {
                swap_color(w, x->parent);
                left_rotate(tree, x->parent);
                w = x->parent->right;
            }
            if(w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else 
            {
                if(w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(tree, x->parent);
                x = tree->root;
            }
        }
        else
        {
            w = x->parent->left;
            if(w->color == RED)
            {
                swap_color(w, x->parent);
                right_rotate(tree, x->parent);
                w = x->parent->left;
            }
            if(w->right->color == BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else 
            {
                if(w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(tree, w);
                    w = x->parent->left;
                }
            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->left->color = BLACK;
            right_rotate(tree, x->parent);
            x = tree->root;
            }
        }
    }
    x->color = BLACK;
    return;
}

void RBTree_remove_node(RBTree *tree, node *x)
{
    node *y = x;
    node *z;
    Color y_original_color = y->color;
    if(x->left == tree->nil)
    {
        z = x->right;
        RBTransplant(tree, x, x->right);
    }
    else if(x->right == tree->nil)
    {
        z = x->left;
        RBTransplant(tree, x, x->left);
    }
    else
    {
        y = minimal_node(tree, x->right, NULL);
        y_original_color = y->color;
        z = y->right;
        if(y->parent == x)
            z->parent = y;
        else
        {
            RBTransplant(tree, y, y->right);
            y->right = x->right;
            y->right->parent = y;
        }
        RBTransplant(tree, x, y);
        y->left = x->left;
        y->left->parent = y;
        y->color = x->color;
    }
    if(y_original_color == BLACK)
    delete_fixup(tree, z);
    tree->nil->left = tree->nil;
    tree->nil->right = tree->nil;
    tree->nil->parent = tree->nil;
    free(x);
    return;
}

node *RBTree_successor(RBTree *tree, node *n)
{
    if(n == tree->nil) return n;

    if(n->right != tree->nil)
        return minimal_node(tree, n->right, NULL);
    node *p = n->parent;
    while (p != tree->nil && n == p->right)
    {
        n = p;
        p = p->parent;
    }
    return p;   
}


void RBTree_clear(RBTree *tree, accept_to_data data_function)
{
    if (tree->root == tree->nil)
        return;

    node *current = tree->root;
    node *prev = tree->nil;

    while (current != tree->nil)
    {
        // Если есть левый ребенок и мы не возвращаемся из него
        if (current->left != tree->nil && prev != current->left && prev != current->right)
        {
            // Идем в левое поддерево
            prev = current;
            current = current->left;
        }
        // Иначе если есть правый ребенок и мы не возвращаемся из него
        else if (current->right != tree->nil && prev != current->right)
        {
            // Идем в правое поддерево
            prev = current;
            current = current->right;
        }
        // Иначе это лист или узел с обработанными детьми - удаляем
        else
        {
            // Сохраняем родителя
            node *parent = current->parent;

            // Освобождаем данные
            if (data_function != NULL)
            {
                data_function(current->data);
            }

            // Отсоединяем от родителя
            if (parent != tree->nil)
            {
                if (parent->left == current)
                {
                    parent->left = tree->nil;
                }
                else
                {
                    parent->right = tree->nil;
                }
            }

            // Удаляем узел
            free(current);

            // Поднимаемся к родителю
            prev = current;
            current = parent;
        }
    }

    tree->root = tree->nil;
}


void RBTree_walk(RBTree *tree, accept_to_data data_function)
{
    node *cur = minimal_node(tree, tree->root, NULL);
    
    while (cur != tree->nil)
    {
        if (data_function != NULL)
            data_function(cur->data);

        cur = RBTree_successor(tree, cur);
    }
}