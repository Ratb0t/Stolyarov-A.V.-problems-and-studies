#include <stdio.h>
#include "RBTree.h"

extern int silent;
// Вспомогательная функция для проверки черной высоты
// Возвращает черную высоту поддерева или -1 в случае нарушения
int checkBlackHeight(RBTree *tree, node *current, int *valid)
{
    // Если уже нашли ошибку, прекращаем проверку
    if (!(*valid))
    {
        return 0;
    }

    // Nil узел (ссылается сам на себя)
    if (current == tree->nil)
    {
        // Nil узел всегда черный и не учитывается в высоте
        return 0;
    }

    // Рекурсивно проверяем левое поддерево
    int leftHeight = checkBlackHeight(tree, current->left, valid);
    if (!(*valid))
    {
        return 0;
    }

    // Рекурсивно проверяем правое поддерево
    int rightHeight = checkBlackHeight(tree, current->right, valid);
    if (!(*valid))
    {
        return 0;
    }

    // Проверка на два подряд красных узла
    if (current->color == RED)
    {
        // Если текущий узел красный, его дети должны быть черными
        if (current->left != tree->nil && current->left->color == RED)
        {
            *valid = 0;
            if(!silent)
                printf("Ошибка: два красных узла подряд (узел %d и левый ребенок)\n", current->key);
            return 0;
        }
        if (current->right != tree->nil && current->right->color == RED)
        {
            *valid = 0;
            if (!silent)
                printf("Ошибка: два красных узла подряд (узел %d и правый ребенок)\n", current->key);
            return 0;
        }
    }

    // Проверка равенства черных высот левого и правого поддеревьев
    if (leftHeight != rightHeight)
    {
        *valid = 0;
        if (!silent)
            printf("Ошибка: черная высота не сбалансирована в узле %d (левая: %d, правая: %d)\n",
                   current->key, leftHeight, rightHeight);
        return 0;
    }

    // Возвращаем черную высоту этого поддерева
    // Учитываем черный цвет текущего узла (кроме корня, если он не учитывается)
    if (current->color == BLACK)
    {
        return leftHeight + 1;
    }
    else
    {
        return leftHeight; // Красные узлы не увеличивают черную высоту
    }
}

// Основная функция проверки сбалансированности КЧ дерева
int isRBTreeBalanced(RBTree *tree)
{
    if (tree == NULL || tree->root == NULL || tree->nil == NULL)
    {
        if (!silent)
            printf("Ошибка: некорректное дерево\n");
        return 0;
    }

    int valid = 1;

    // Проверка свойства 1: Корень должен быть черным
    if (tree->root != tree->nil && tree->root->color != BLACK)
    {
        if (!silent)
            printf("Ошибка: корень дерева не черный\n");
        return 0;
    }

    // Проверка черной высоты и других свойств
    // Корень не учитывается в черной высоте согласно требованию
    int blackHeight = checkBlackHeight(tree, tree->root, &valid);

    if (valid)
    {
        if (!silent)
            printf("Дерево сбалансировано. Черная высота (без учета корня): %d\n", blackHeight);
    }
    else
    {
        if (!silent)
            printf("Дерево не сбалансировано\n");
    }

    return valid;
}

// Пример использования:
/*
int main() {
    RBTree tree;
    // ... инициализация дерева ...

    if (isRBTreeBalanced(&tree)) {
        printf("Красно-черное дерево сбалансировано\n");
    } else {
        printf("Красно-черное дерево НЕ сбалансировано\n");
    }

    return 0;
}
*/