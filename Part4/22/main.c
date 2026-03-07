/*
Я не смог реализовать красно-черное дерево
вставка была была коректная, но удаление элементов
нет, поэтому всю реализацию в гневном порыве я уничтожил
*/
#include "RBTree.h"
#include <stdio.h>
#include <stdlib.h>
#define RANDOM(lower_range, upper_range) \
    ((lower_range) + (int)(((float)(upper_range) * rand()) / (RAND_MAX + 1.0)))

#define RAND
//#define Single_delete
int silent = 0;

int compare_int(top_type a, top_type b)
{
    if(a.as_int == b.as_int)
        return compare_equal;
    if(a.as_int > b.as_int)
        return compare_great;
    
    return compare_less;
}

int print_data(top_type data)
{
    return printf("%d\n", data.as_int);
}

int test_tree(char *count)
{
    int ret_code = 1;
    RBTree *tree = create_tree(compare_int);
    node *delete;
    int size;
#ifdef Fix
    // int simple_data[] = {20, 10, 25, 4, 16, 23, 30, 31, 2, 5, 14, 17, 3, 
    //     12, 15, 19, 11};
    int simple_data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    size = sizeof(simple_data) / sizeof(int);
#   ifdef Single_delete
    int on_remove = atoi(count);
#   endif
#else
    size = atoi(count);
#endif

    
#ifdef RAND
    if(size > 10000)
    {
        printf("When the value is greater than 10000, "
        "tests with a random value slow down significantly.\n");
        return 1;
    }
    for(int i = 0; i < size;)
    {
        int val = RANDOM(1, size);
        //printf("%d ", val);
        if (RBTree_insert(tree, (top_type)val))
            ++i;
    }
#endif

#ifndef RAND
    for (int i = 0; i < size; ++i)
    {
#   ifdef Fix
        RBTree_insert(tree, (top_type){.as_int = simple_data[i]});
#   endif /*Fix*/

#   ifdef Direction_right
        RBTree_insert(tree, (top_type){.as_int = (i + 1)});
#   endif /*Direction_right*/

#   ifdef Direction_left
        RBTree_insert(tree, (top_type){.as_int = -(i + 1)});
#   endif /*Direction_left*/
    }
#endif /*!RAND*/

    if ((ret_code = isRBTreeBalanced(tree)))
    {
        if (!silent)
            printf("Красно-черное дерево сбалансировано\n");
    }
    else
    {
        if (!silent)
            printf("Красно-черное дерево НЕ сбалансировано при вставке\n");
        return !ret_code;
    }

    RBTree_walk(tree, print_data);

#ifdef Single_delete
    delete = RBTree_find_node(tree, (top_type){.as_int = on_remove});
    if(delete)RBTree_remove_node(tree, delete);
    if ((ret_code = isRBTreeBalanced(tree)))
    {
        if (!silent)
            printf("Красно-черное дерево сбалансировано\n");
    }
    else
    {
        if (!silent)
            printf("Красно-черное дерево НЕ сбалансировано при удалении\n");
    }
#else
    for (int i = 1; i < size; ++i)
    {

#ifdef Fix
        delete = RBTree_find_node(tree, (top_type){.as_int = simple_data[i]});
#endif /*Fix*/

#ifdef Direction_right
        delete = RBTree_find_node(tree, (top_type){.as_int = i});
#endif /*Direction_right*/

#ifdef Direction_left
        delete = RBTree_find_node(tree, (top_type){.as_int= -i});
#endif /*Direction_left*/
#ifdef Single_delete
        RBTree_remove_node(tree, delete);
#endif
        if ((ret_code = isRBTreeBalanced(tree)))
        {
            if (!silent)
                printf("Красно-черное дерево сбалансировано\n");
        }
        else
        {
            if (!silent)
                printf("Красно-черное дерево НЕ сбалансировано при удалении\n");
            return !ret_code;
        }
    }
#ifdef Fix
    delete = RBTree_find_node(tree, (top_type){.as_int = simple_data[0]});
#else
    printf("Will removed %d\n", size);
    delete = RBTree_find_node(tree, (top_type){.as_int = size});
#endif
    RBTree_remove_node(tree, delete);
#endif
    RBTree_walk(tree, print_data);
    RBTree_clear(tree, NULL);
    destroy_tree(tree);
    return !ret_code;

}


int main(int argc, char *argv[])
{
#ifndef Fix
    if(argc < 2)
    {
        printf("Input in args iteration/node count for test\n");
        return 1;
    }
    int count = 1;
    if(argc == 3)
    {
        if(argv[1][0] == '-')
        {
            count = 2;
            if(argv[1][1] == 's')
                silent = 1;
        }
        else if(argv[2][0] == '-')
            if (argv[2][1] == 's')
                silent = 1;
    }
    return test_tree(argv[count]);
#else
    if(argc != 2)
    {
        printf("Input in args withc key remove\n");
        return 1;
    }
    return test_tree(argv[1]);
#endif
}