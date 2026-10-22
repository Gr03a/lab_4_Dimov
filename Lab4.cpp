#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define MAX_LEVEL 11

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

Node* root = NULL;

// Создание нового узла
Node* createNode(int data) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (n == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    return n;
}

// Добавление элемента с дубликатами (с ограничением уровней)
Node* insert(Node* tree, int data, int level) {
    if (level >= MAX_LEVEL) {
        printf("Достигнут максимум уровней (%d). %d не добавлено.\n",
            MAX_LEVEL, data);
        return tree;
    }

    if (tree == NULL)
        return createNode(data);

    if (data < tree->data)
        tree->left = insert(tree->left, data, level + 1);
    else
        tree->right = insert(tree->right, data, level + 1);

    return tree;
}

// Добавление без дубликатов (с ограничением уровней)
Node* insertUnique(Node* tree, int data, int level) {
    if (level >= MAX_LEVEL) {
        printf("Достигнут максимум уровней (%d). %d не добавлено.\n",
            MAX_LEVEL, data);
        return tree;
    }

    if (tree == NULL)
        return createNode(data);

    if (data < tree->data)
        tree->left = insertUnique(tree->left, data, level + 1);
    else if (data > tree->data)
        tree->right = insertUnique(tree->right, data, level + 1);

    return tree;
}

// Вывод дерева с нумерацией уровней
void printTree(Node* tree, int level) {
    if (tree == NULL)
        return;

    printTree(tree->right, level + 1);

    for (int i = 0; i < level; i++)
        printf("    ");

    printf("[%d] %d\n", level, tree->data);

    printTree(tree->left, level + 1);
}

// Поиск значения — возвращает уровень или -1
int search(Node* tree, int value, int level) {
    if (tree == NULL)
        return 0;

    int found = 0;

    if (tree->data == value) {
        printf("  Найдено %d на уровне %d\n", value, level);
        found = 1;
    }

    return found
        + search(tree->left, value, level + 1)
        + search(tree->right, value, level + 1);
}

// Подсчёт вхождений с выводом уровней
int countOccurrences(Node* tree, int value, int level) {
    if (tree == NULL)
        return 0;

    int count = 0;

    if (tree->data == value) {
        printf("  Найдено %d на уровне %d\n", value, level);
        count = 1;
    }

    return count
        + countOccurrences(tree->left, value, level + 1)
        + countOccurrences(tree->right, value, level + 1);
}

// Освобождение памяти
void freeTree(Node* tree) {
    if (tree != NULL) {
        freeTree(tree->left);
        freeTree(tree->right);
        free(tree);
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, "ru-RU.UTF-8");
    int choice, value;

    do {
        printf("\n===== МЕНЮ =====\n");
        printf("1. Добавить элементы (с дубликатами)\n");
        printf("2. Вывести дерево\n");
        printf("3. Поиск значения\n");
        printf("4. Подсчёт вхождений значения\n");
        printf("5. Добавить элементы без дубликатов\n");
        printf("0. Выход\n");
        printf("Выберите пункт: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Вводите целые числа (окончание — -1):\n");
            while (1) {
                scanf("%d", &value);
                if (value == -1) break;
                root = insert(root, value, 0);
            }
            break;

        case 2:
            if (root == NULL)
                printf("Дерево пусто.\n");
            else
                printTree(root, 0);
            break;

        case 3: {
            printf("Введите значение для поиска: ");
            scanf("%d", &value);
            int lvl = search(root, value, 0);
            if (lvl < 0)
                printf("Значение %d не найдено.\n", value);
            break;
        }

        case 4: {
            printf("Введите значение для подсчёта: ");
            scanf("%d", &value);
            printf("Вхождения значения %d:\n", value);
            int cnt = countOccurrences(root, value, 0);
            printf("Итого: %d раз(а).\n", cnt);
            break;
        }

        case 5:
            printf("Вводите целые числа без дубликатов (окончание — -1):\n");
            while (1) {
                scanf("%d", &value);
                if (value == -1) break;
                root = insertUnique(root, value, 0);
            }
            break;

        case 0:
            printf("Выход из программы.\n");
            break;

        default:
            printf("Неверный пункт меню.\n");
        }
    } while (choice != 0);

    freeTree(root);
    return 0;
}