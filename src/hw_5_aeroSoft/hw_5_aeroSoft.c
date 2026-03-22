#include "hw_5_aeroSoft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// СЛУЖЕБНЫЕ ФУНКЦИИ

// Создание узла
static Node* createNode(const char* code, const char* name)
{
    Node* newNode = malloc(sizeof(Node));
    if (!newNode)
        return NULL;
    strncpy(newNode->code, code, 3);
    newNode->code[3] = '\0';
    strncpy(newNode->name, name, sizeof(newNode->name) - 1);
    newNode->name[sizeof(newNode->name) - 1] = '\0';
    newNode->height = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Удаление узла
static void freeNode(Node* node)
{
    if (node == NULL)
        return;
    freeNode(node->left);
    freeNode(node->right);
    free(node);
}

// Нахождение высоты
static int treeHeight(Node* node)
{
    return node ? node->height : 0;
}

// Нахождение максимума
static int max(int a, int b)
{
    return (a > b) ? a : b;
}

// Правый поворот для балансировки
static Node* rightRotate(Node* oldRoot)
{
    Node* newRoot = oldRoot->left; // новый корень — левый ребёнок
    Node* subtree = newRoot->right; // поддерево, которое перейдёт к старому корню

    newRoot->right = oldRoot;
    oldRoot->left = subtree;

    oldRoot->height = max(treeHeight(oldRoot->left), treeHeight(oldRoot->right)) + 1;
    newRoot->height = max(treeHeight(newRoot->left), treeHeight(newRoot->right)) + 1;

    return newRoot;
}

// Левый поворот для балансировки
static Node* leftRotate(Node* oldRoot)
{
    Node* newRoot = oldRoot->right; // новый корень — правый ребёнок
    Node* subtree = newRoot->left; // поддерево, которое перейдёт к старому корню

    newRoot->left = oldRoot;
    oldRoot->right = subtree;

    oldRoot->height = max(treeHeight(oldRoot->left), treeHeight(oldRoot->right)) + 1;
    newRoot->height = max(treeHeight(newRoot->left), treeHeight(newRoot->right)) + 1;

    return newRoot;
}

// Вставка узла
static Node* insertNode(Node* node, const char* code, const char* name)
{
    if (node == NULL)
        return createNode(code, name);

    int compare = strcmp(code, node->code);
    if (compare < 0)
        node->left = insertNode(node->left, code, name);
    else if (compare > 0)
        node->right = insertNode(node->right, code, name);
    else
        return node; // already exist

    node->height = 1 + max(treeHeight(node->left), treeHeight(node->right));
    int balance = treeHeight(node->left) - treeHeight(node->right);

    // LL
    if (balance > 1 && node->left && strcmp(code, node->left->code) < 0)
        return rightRotate(node);

    // RR
    if (balance < -1 && node->right && strcmp(code, node->right->code) > 0)
        return leftRotate(node);

    // LR
    if (balance > 1 && node->left && strcmp(code, node->left->code) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RL
    if (balance < -1 && node->right && strcmp(code, node->right->code) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

// Нахождение минимального узла
static Node* findMinNode(Node* node)
{
    while (node && node->left)
        node = node->left;
    return node;
}

// Удаление узла
static Node* deleteNode(Node* node, const char* code)
{
    if (node == NULL)
        return NULL;

    int compare = strcmp(code, node->code);
    if (compare < 0)
        node->left = deleteNode(node->left, code);
    if (compare > 0)
        node->right = deleteNode(node->right, code);
    else {
        if (node->left == NULL) {
            Node* rightChild = node->right;
            free(node);
            return rightChild;
        } else if (node->right == NULL) {
            Node* leftChild = node->left;
            free(node);
            return leftChild;
        } else {
            // Узел с двумя детьми
            Node* minNode = findMinNode(node->right);
            // Копируем данные
            strncpy(node->code, minNode->code, sizeof(node->code) - 1);
            node->code[sizeof(node->code) - 1] = '\0';
            strncpy(node->name, minNode->name, sizeof(node->name) - 1);
            node->name[sizeof(node->name) - 1] = '\0';
            // Удаляем минимальный узел из правого поддерева
            node->right = deleteNode(node->right, minNode->code);
        }
    }

    // Обновляем высоту
    node->height = 1 + max(treeHeight(node->left), treeHeight(node->right));
    int balance = treeHeight(node->left) - treeHeight(node->right);

    // LL
    if (balance > 1 && node->left && treeHeight(node->left->left) >= treeHeight(node->left->right))
        return rightRotate(node);

    // LR
    if (balance > 1 && node->left && treeHeight(node->left->left) < treeHeight(node->left->right)) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RR
    if (balance < -1 && node->right && treeHeight(node->right->right) >= treeHeight(node->right->left))
        return leftRotate(node);

    // RL
    if (balance < -1 && node->right && treeHeight(node->right->right) < treeHeight(node->right->left)) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// ПОЛЬЗОВАТЕЛЬСКИЕ ФУНКЦИИ

// Поиск записи
char* treeSearch(Tree* tree, const char* code)
{
    if (!tree) {
        char* buf = malloc(150);
        if (buf)
            sprintf(buf, "Аэропорт с кодом '%s' не найден в базе.", code);
        return buf;
    }
    Node* cur = tree->root;
    while (cur) {
        int cmp = strcmp(code, cur->code);
        if (cmp == 0) {
            size_t len = strlen(cur->name);
            char* buf = malloc(len + 1);
            if (buf)
                memcpy(buf, cur->name, len + 1);
            return buf;
        } else if (cmp < 0) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    char* buf = malloc(150);
    if (buf)
        sprintf(buf, "Аэропорт с кодом '%s' не найден.", code);
    return buf;
}

// Вставка записи
void treeInsert(Tree* tree, const char* code, const char* name)
{
    if (tree)
        tree->root = insertNode(tree->root, code, name);
}

// Удаление записи
void treeRemove(Tree* tree, const char* code)
{
    if (tree)
        tree->root = deleteNode(tree->root, code);
}

// Создание дерева
Tree* createTree(void)
{
    Tree* newTree = (Tree*)malloc(sizeof(Tree));
    if (newTree) {
        newTree->root = NULL;
    }
    return newTree;
}

// Удаление дерева
void treeFree(Tree* tree)
{
    if (tree == NULL)
        return;
    freeNode(tree->root);
    free(tree);
}
