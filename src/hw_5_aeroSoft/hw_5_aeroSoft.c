#include "hw_5_aeroSoft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура узла
typedef struct Node {
    char code[4];
    char name[512];
    struct Node* left;
    struct Node* right;
    int height;
} Node;

// Структура дерева
struct Tree {
    Node* root;
};

// СЛУЖЕБНЫЕ ФУНКЦИИ

// Поиск максимума двух чисел
static int max(int a, int b)
{
    return a > b ? a : b;
}

// Получение высоты узла
static int nodeHeight(Node* node)
{
    return node ? node->height : 0;
}

// Обновление высоты узла
static void updateHeight(Node* node)
{
    if (node)
        node->height = 1 + max(nodeHeight(node->left), nodeHeight(node->right));
}

// Малое правое вращение (RR)
static Node* rotateRight(Node* oldRoot)
{
    Node* newRoot = oldRoot->left;
    Node* subtree = newRoot->right;
    newRoot->right = oldRoot;
    oldRoot->left = subtree;
    updateHeight(oldRoot);
    updateHeight(newRoot);
    return newRoot;
}

// Малое левое вращение (LL)
static Node* rotateLeft(Node* oldRoot)
{
    Node* newRoot = oldRoot->right;
    Node* subtree = newRoot->left;
    newRoot->left = oldRoot;
    oldRoot->right = subtree;
    updateHeight(oldRoot);
    updateHeight(newRoot);
    return newRoot;
}

// Балансировка после вставки
static Node* balanceInsert(Node* node, const char* code)
{
    int balance = nodeHeight(node->left) - nodeHeight(node->right);
    // LL
    if (balance > 1 && node->left && strcmp(code, node->left->code) < 0)
        return rotateRight(node);
    // RR
    if (balance < -1 && node->right && strcmp(code, node->right->code) > 0)
        return rotateLeft(node);
    // LR
    if (balance > 1 && node->left && strcmp(code, node->left->code) > 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    // RL
    if (balance < -1 && node->right && strcmp(code, node->right->code) < 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

// Балансировка после удаления
static Node* balanceDelete(Node* node)
{
    int balance = nodeHeight(node->left) - nodeHeight(node->right);
    // LL
    if (balance > 1 && nodeHeight(node->left->left) >= nodeHeight(node->left->right))
        return rotateRight(node);
    // LR
    if (balance > 1 && nodeHeight(node->left->left) < nodeHeight(node->left->right)) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    // RR
    if (balance < -1 && nodeHeight(node->right->right) >= nodeHeight(node->right->left))
        return rotateLeft(node);
    // RL
    if (balance < -1 && nodeHeight(node->right->right) < nodeHeight(node->right->left)) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

// Создание нового узла
static Node* createNode(const char* code, const char* name)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node)
        return NULL;
    strncpy(node->code, code, 3);
    node->code[3] = '\0';
    strncpy(node->name, name, sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Вставка узла
static Node* insertNode(Node* node, const char* code, const char* name)
{
    if (!node)
        return createNode(code, name);
    int cmp = strcmp(code, node->code);
    if (cmp < 0)
        node->left = insertNode(node->left, code, name);
    else if (cmp > 0)
        node->right = insertNode(node->right, code, name);
    else
        return node;
    updateHeight(node);
    return balanceInsert(node, code);
}

// Поиск минимального узла в поддереве
static Node* findMin(Node* node)
{
    while (node && node->left)
        node = node->left;
    return node;
}

// Удаление узла
static Node* deleteNode(Node* node, const char* code)
{
    if (!node)
        return NULL;
    int cmp = strcmp(code, node->code);
    if (cmp < 0)
        node->left = deleteNode(node->left, code);
    else if (cmp > 0)
        node->right = deleteNode(node->right, code);
    else {
        if (!node->left || !node->right) {
            Node* child = node->left ? node->left : node->right;
            free(node);
            return child;
        } else {
            Node* minNode = findMin(node->right);
            strcpy(node->code, minNode->code);
            strcpy(node->name, minNode->name);
            node->right = deleteNode(node->right, minNode->code);
        }
    }
    updateHeight(node);
    return balanceDelete(node);
}

// Печать узла
static void printNode(Node* node)
{
    if (!node)
        return;
    printf("%s : %s\n", node->code, node->name);
    printNode(node->left);
    printNode(node->right);
}

// Печать дерева
static void treePrint(Tree* tree)
{
    if (tree)
        printNode(tree->root);
}

// Освобождение узлов
static void freeNodes(Node* node)
{
    if (!node)
        return;
    freeNodes(node->left);
    freeNodes(node->right);
    free(node);
}

// Сохранение узла
static void saveNode(const Node* node, FILE* f)
{
    if (!node)
        return;
    saveNode(node->left, f);
    fprintf(f, "%s:%s\n", node->code, node->name);
    saveNode(node->right, f);
}

// ПОЛЬЗОВАТЕЛЬСКИЕ ФУНКЦИИ

// Создание дерева
Tree* createTree(void)
{
    return calloc(1, sizeof(Tree));
}

// Удаление дерева
void treeFree(Tree* tree)
{
    if (tree) {
        freeNodes(tree->root);
        free(tree);
    }
}

// Вставка записи
bool treeInsert(Tree* tree, const char* code, const char* name)
{
    if (tree) {
        tree->root = insertNode(tree->root, code, name);
        return true;
    } else
        return false;
}

// Поиск записи
// Return: указатель на name или NULL
const char* treeSearch(Tree* tree, const char* code)
{
    if (!tree)
        return NULL;
    Node* current = tree->root;
    while (current) {
        int cmp = strcmp(code, current->code);
        if (cmp == 0)
            return current->name;
        if (cmp < 0)
            current = current->left;
        else
            current = current->right;
    }
    return NULL;
}

// Удаление записи
void treeRemove(Tree* tree, const char* code)
{
    if (tree)
        tree->root = deleteNode(tree->root, code);
}

// Сохранение базы
void treeSave(const Tree* tree, FILE* file)
{
    saveNode(tree->root, file);
}
