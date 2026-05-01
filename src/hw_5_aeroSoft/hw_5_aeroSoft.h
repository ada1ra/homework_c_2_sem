#include <stdio.h>
#pragma once

typedef struct Tree Tree;

// Создание дерева
Tree* createTree(void);

// Удаление дерева
void treeFree(Tree* tree);

// Вставка записи
bool treeInsert(Tree* tree, const char* code, const char* name);

// Поиск записи
// Return: указатель на name или NULL
const char* treeSearch(Tree* tree, const char* code);

// Удаление записи
void treeRemove(Tree* tree, const char* code);

// Сохранение базы
void treeSave(const Tree* tree, FILE* file); 
