#include "RBTree.h"
#include <stdlib.h>

node *RBTree_find_node(RBTree *tree, int key)
{
    node *n = tree->root;
    while (n != tree->nil)
    {
        if (n->key == key)
            return n;
        else if (n->key > key)
            n = n->left;
        else
            n = n->right;
    }

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

int insert(RBTree *tree, int key)
{
    node *cur = tree->root;
    node *cur_parent = tree->nil;
    while (cur != tree->nil)
    {
        cur_parent = cur;
        if (cur->key == key)
        {
            tree->root->color = BLACK;
            return 0;
        }

        descending_fixup(tree, cur);
        
        if (cur->key > key)
            cur = cur->left;
        else
            cur = cur->right;
        
    }
    node *new_node = create_node(tree, key);
    new_node->parent = cur_parent;
    if (cur_parent == tree->nil)
        tree->root = new_node;
    else if (cur_parent->key > key)
        cur_parent->left = new_node;
    else
        cur_parent->right = new_node;

    insert_fixup(tree, new_node);

    tree->root->color = BLACK;
    return 1;
}

node *create_node(RBTree *tree, int key)
{
    node *new = malloc(sizeof(node));
    if (!new)
    {
        return NULL;
    }
    new->color = RED;
    new->key = key;
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
    new->key = 0;
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
    tree->function = function;
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

/*1)
    Балансировка дерева и функция choose_witch_delete гарантирует,
    если узел красный, то у него нет детей (невозможно вставить красный
    узел к красному листу произойдет балансировка. Функция choose_witch_delete
    возвращает КРАЙНИЙ лист (т.е. у листа гарантировано нет 1 потомка слева или справа).
    Если возвращаемый лист красный и у него есть 1 потомок, то дерево не сбалансировано
    т.к. возможно два нарушения.
    1) Потомок является красным, у красного родителя не может быть красного потомка.
    2) Т.к. у красного узала потомок обязан быть черным и он является единственным, тогда
    будет нарушена черная высота.
*/
node *choose_witch_delete(RBTree *tree, node *n)
{
    int length_min, length_max;
    node *min_right = minimal_node(tree, n->right, &length_min);
    node *max_left = maximum_node(tree, n->left, &length_max);

    if(length_min > length_max)
        return min_right;
    else
        return max_left;

}
#define MY_DELETE_
#ifdef MY_DELETE
/*
v - замена
x - заменяемый
Функция меняет местами узлы v и x, но поля key и data остаются
*/
void replace_node(RBTree *tree, node *v, node *x)
{
    node* tmp;

    if(v->parent != tree->nil)
    {
        if (v->parent->left == v)
            v->parent->left = x;
        else
            v->parent->right = x;
    }

    if (x->parent != tree->nil)
    {
        if (x->parent->left == x)
            x->parent->left = v;
        else
            x->parent->right = v;
    }

    tmp = v->parent;

    v->parent = x->parent;
    x->parent = tmp;

    if(v->left != tree->nil)
        v->left->parent = x;
    
    if(v->right != tree->nil)
        v->right->parent = x;

    if(x->left != tree->nil)
        x->left->parent = v;

    if(x->right != tree->nil)
        x->right->parent = v;
    

    tmp = v->left;

    v->left = x->left;
    x->left = tmp;

    tmp = v->right;
    
    v->right = x->right;
    x->right = tmp;

    if(tree->root == x)
        tree->root = v;
    else if(tree->root == v)
        tree->root = x;

    swap_color(x, v);
    return;
    
}

/*2)
    Отличие transplant_node от replace_node заключается в том,
    что узел v встает на место узла x, а узел x отрезается от дерева и фактически
    из дерева на него ничто не ссылается. Родительский который указывал на v, начинает
    указывать на левый или правый узел, но т.к. функция фактически используется
    только в удалении красного листа, то фактически родитель будет ссылаться на nil.
    Почему у красного листа не может быть детей см коммент. 1)
*/
void transplant_node(RBTree *tree, node *v, node *x)
{
    if(v == tree->nil)
        return;
    if(v->parent != tree->nil)
    {
        if(v->parent->left == v)
            v->parent->left = v->left;
        else
            v->parent->right = v->right;
    }

    v->left = x->left;
    v->right = x->right;

    if (v->left != tree->nil)
        v->left->parent = v;
    if (v->right != tree->nil)
        v->right->parent = v;

    v->parent = x->parent;
    if (v->parent == tree->nil)
        tree->root = v;
    else if(v->parent->left == x)
        v->parent->left = v;
        else
            v->parent->right = v;
    
    
    swap_color(x,v);
    return;
}

#ifdef REMOVE_WITH_REPLACE_NODE
/*3)
    Почему сделано два варианта.
    Дело в том, чтобы не изменять поле данных, т.к. там может быть
    инициализированная на стеке структура и копировать её может оказаться
    накладно, особенно если портировать эту реализацию на C++ с ее классами,
    где хранится не укзатель на объект, а сам объект, в таком случае будет
    вызываться конструктор копирования, поэтому дешевле заместить удаляемый
    дочерним, чем копировать данные. Вариант без замены подойдет если размер 
    хранимых данных не большой, т.е. указатель или число и т.п. или для множества
*/
void remove_red_leaf(RBTree *tree, node *remove_node, node *replacement_node)
{
#ifndef USE_TRANSPLANT_FUNCTION
    replace_node(tree, replacement_node, remove_node);
    
    //if(remove_node->parent != tree->nil)
    //{
        if(remove_node->parent->left == remove_node)
            remove_node->parent->left = tree->nil;
        else
            remove_node->parent->right = tree->nil;
    //}
#else
    transplant_node(tree, replacement_node, remove_node);
#endif
    free(remove_node);
    return;
}

void remove_black_node(RBTree *tree, node *remove_node, node *replacement_node)
{

    if(replacement_node->left != tree->nil)
        remove_red_leaf(tree, remove_node, replacement_node->left);
    else
        remove_red_leaf(tree, remove_node, replacement_node->right);

    return;
}

void remove_fixup_case1(RBTree *tree, node *n)
{
    n->color = n->parent->color;
    n->parent->color = BLACK;
    if(get_granson_type(tree, n->right) == OUTSIDE_RIGHT)
    {
        n->right->color = BLACK;
        left_rotate(tree, n->parent);
    }
    else
    {
        n->left->color = BLACK;
        right_rotate(tree, n->parent);
    }
    return;
}

void remove_fixup_case2(RBTree *tree, node *n)
{
    if(get_granson_type(tree, n->right) == OUTSIDE_RIGHT)
    {
        swap_color(n, n->left);
        right_rotate(tree, n);
    }
    else
    {
        swap_color(n, n->right);
        left_rotate(tree, n);
    }

    return;
}

void remove_fixup_case3(RBTree *tree, node *n)
{
    if(n->parent == tree->root)
    {
        n->color = RED;
        return;
    }
    if(n->parent->color == RED)
    {
        swap_color(n->parent, n);
        return;
    }
}

void remove_fixup(RBTree *tree, node *cur)
{
    if (cur->left == tree->nil && cur->left == tree->nil)
    {
        remove_fixup_case3(tree, cur);
    }

    if((cur->right->color == BLACK && get_granson_type(tree, cur->right) == OUTSIDE_RIGHT && cur->left->color == RED) ||
        (cur->left->color == BLACK && get_granson_type(tree, cur->left) == OUTSIDE_LEFT && cur->right->color == RED))
    {
        remove_fixup_case2(tree, cur);
        cur = cur->parent;
    }

    if ((cur->right->color == RED && get_granson_type(tree, cur->right) == OUTSIDE_RIGHT)||
    (cur->left->color == RED && get_granson_type(tree, cur->right) == OUTSIDE_LEFT))
    {
        remove_fixup_case1(tree, cur);
    }
}   

void remove_black_leaf(RBTree *tree, node *remove_node, node *replacement_node)
{
    if (replacement_node == tree->nil && tree->root == remove_node)
    {
        tree->root = tree->nil;
        free(remove_node);
        return;
    }

    node *brother;
    if(replacement_node->parent->left == replacement_node)
        brother = replacement_node->parent->right;
    else
        brother = replacement_node->parent->left;
    
    
#ifndef USE_TRANSPLANT_FUNCTION
    replace_node(tree, remove_node, replacement_node);
#else
    transplant_node(tree, replacement_node, remove_node);
#endif

    free(remove_node);

    remove_fixup(tree, brother);

    return;
}

void delete_node(RBTree *tree, node *x)
{
    
    node *replacement_node = choose_witch_delete(tree, x);
    if (replacement_node->color == RED && (x->left->left == tree->nil && x->right == tree->nil))
    {
        /*Будет удален x, а replacement_node станет на его место*/
        remove_red_leaf(tree, x, replacement_node);
        return;
    }
    /*Удаляем черный узел с одним потомком*/
    if (replacement_node->left != tree->nil || replacement_node->right != tree->nil)
    {
        remove_black_node(tree, x, replacement_node);
        return;
    }

    /*Удаляем черный лист*/
    remove_black_leaf(tree, x, replacement_node);
    return;
}

#else
void remove_red_leaf(RBTree *tree, node *x, node *remove_node)
{
    x->key = remove_node->key;
    x->data = remove_node->data;
    if(remove_node->parent->left = remove_node)
        remove_node->parent->left = tree->nil;
    else
        remove_node->parent->right = tree->nil;

    free(remove_node);
    return;
}

void remove_black_node(RBTree *tree, node *x, node *remove_node)
{
    
    return;
}

void remove_black_leaf(RBTree *tree, node *x, node *remove_node)
{

    return;
}

void delete_node(RBTree *tree, node *x)
{

    node *remove_node = choose_witch_delete(tree, x);

    if (remove_node->color == RED)
    {

        /*Будет удален remove_node, а x примет значение key и data*/
        remove_red_leaf(tree, x, remove_node);
    }
    /*Удаляем черный узел с одним потомком*/
    if (remove_node->left != tree->nil || remove_node->right != tree->nil)
    {
        remove_black_node(tree, x, remove_node);
    }

    /*Удаляем черный лист*/
    remove_black_leaf(tree, x, remove_node);
    return;
}
#endif
#else
void RBTransplant(RBTree *tree, node *x, node *v)
{
    if (x->parent == tree->nil)
        tree->root = v;
    else if (x == x->parent->left)
        x->parent->left = v;
    else
        x->parent->right = v;
    //if(v->parent != tree->nil)
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
#endif /*MY DELETE*/
