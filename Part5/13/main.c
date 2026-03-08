#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include "my_rbtree.h"

typedef char **string_array;

struct dictionary
{
    int key;
    char *name;
};

int compare_dictionary(top_type v1, top_type v2)
{
    int a = ((struct dictionary *)v1.as_void)->key;
    int b = ((struct dictionary *)v2.as_void)->key;

    if (a == b)
        return compare_equal;
    if (a > b)
        return compare_great;
    return compare_less;
}

int main(int argc, char *argv[])
{
    RBTree *tree = RBTree_create(compare_dictionary);

    /*По умолчанию argv[argc] == NULL*/
    int pos = 1;
    for (int i = 1; i <= argc; ++i)
    {
        if (argc == i || (argv[i][0] == ';' && argv[i][1] == ';'))
        {
            char *old = argv[i];
            argv[i] = NULL;
            struct dictionary *dict = malloc(sizeof(struct dictionary));
            dict->name = argv[pos];
            dict->key = fork();
            if (dict->key == 0)
            {
                if (dict->name)
                    execvp(dict->name, argv + pos);
                //printf("%s not exect\n", dict->name);
                free(dict);
                RBTree_destroy(tree, (accept_to_data)free);
                exit(1);
            }
            RBTree_insert(tree, (top_type){.as_void = dict});
            argv[i] = old;
            pos = i + 1;
        }
    }

    while (!RBTree_is_empty(tree))
    {
        struct dictionary d;
        int status;
        d.key = wait(&status);
        node *n = RBTree_find(tree, (top_type){.as_void = &d});
        if (n)
        {
            if (n->data.as_void)
            {
                if (WEXITSTATUS(status) == 0)
                    printf("Successul: %s\n", ((struct dictionary *)(n->data.as_void))->name);
                free(n->data.as_void);
            }
                
            RBTree_remove_node(tree, n);
        }
    }

    RBTree_destroy(tree, NULL);

    return 0;
}