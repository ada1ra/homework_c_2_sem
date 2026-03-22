#pragma once

// Структура узла
typedef struct Node {
    char code[4];
    char name[256];
    struct Node* left;
    struct Node* right;
    int height;
} Node;

// Структура дерева
typedef struct {
    Node* root;
} Tree;

// Создание дерева
Tree* createTree(void);
// Удаление дерева
void treeFree(Tree* tree);

// Вставка записи
void treeInsert(Tree* tree, const char* code, const char* name);
// Поиск записи
char* treeSearch(Tree* tree, const char* code);
// Удаление записи
void treeRemove(Tree* tree, const char* code);
