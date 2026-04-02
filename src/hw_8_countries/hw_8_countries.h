#pragma once

// Создание графа
void createGraph(int n, int m);

// Добавление дороги
void addRoad(int u, int v, int w);

// Запуск распределения
void runDistribution(int k, const int* capitals);

// Получение указателя на массив городов государства и их количества
int* getStateCities(int state, int* count);

// Освобождение памяти
void freeMemory(void);