#include "hw_8_countries.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100000 // максимальное число городов
#define MAXM 200000 // максимальное число дорог

// Структура для хранения ребра
typedef struct Edge {
    int to, len;
    struct Edge* next;
} Edge;

// Структура для элемента кучи
typedef struct {
    int dist; // расстояние от столицы
    int city; // номер города
    int state; // номер государства (1..k)
} Entry;

// Глобальные массивы
Edge* head[MAXN + 1]; // списки смежности
int visited[MAXN + 1]; // занят ли город (0/1)
int belong[MAXN + 1]; // к какому государству принадлежит
int stateListSize[MAXN + 1]; // количество городов в каждом государстве
int* stateList[MAXN + 1]; // массив городов для каждого государства

// Куча
Entry* heap = NULL;
int heapSize = 0;
int heapCap = 0;
int totalCities = 0;
int totalStates = 0;

// Добавление в кучу
static void heapPush(Entry element)
{
    if (heapSize >= heapCap) {
        int newCap = heapCap ? heapCap * 2 : 16;
        Entry* newHeap = realloc(heap, newCap * sizeof(Entry));
        if (!newHeap) {
            printf("Ошибка выделения памяти для кучи\n");
            exit(1);
        }
        heap = newHeap;
        heapCap = newCap;
    }

    int i = heapSize++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[p].dist <= element.dist)
            break;
        heap[i] = heap[p];
        i = p;
    }

    heap[i] = element;
}

// Удаление из кучи
static Entry heapPop(void)
{
    Entry top = heap[0];
    Entry last = heap[--heapSize];
    int i = 0;

    while (1) {
        int left = i * 2 + 1;
        int right = i * 2 + 2;
        int best = i;

        if (left < heapSize && heap[left].dist < last.dist)
            best = left;
        if (right < heapSize && heap[right].dist < heap[best].dist)
            best = right;
        if (best == i)
            break;

            heap[i] = heap[best];
        i = best;
    }
    heap[i] = last;
    return top;
}

// Создание графа
void createGraph(int n, int m)
{
    totalCities = n;
    for (int i = 1; i <= n; i++) {
        head[i] = NULL;
        visited[i] = 0;
        belong[i] = 0;
    }
}

// Добавление дороги
void addRoad(int u, int v, int w)
{
    Edge* element1 = malloc(sizeof(Edge));
    if (!element1) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }
    element1->to = v;
    element1->len = w;
    element1->next = head[u];
    head[u] = element1;

    Edge* element2 = malloc(sizeof(Edge));
    if (!element2) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }
    element2->to = u;
    element2->len = w;
    element2->next = head[v];
    head[v] = element2;
}

// Запуск распределения
void runDistribution(int k, const int* capitals)
{
    totalStates = k;

    // Подготовка списков государств
    for (int i = 1; i <= k; i++) {
        stateList[i] = malloc(sizeof(int) * (totalCities + 1));
        if (!stateList[i]) {
            printf("Ошибка выделения памяти\n");
            exit(1);
        }
        stateListSize[i] = 0;
    }

    // Инициализация кучи столицами
    heap = NULL;
    heapSize = 0;
    heapCap = 0;
    for (int i = 0; i < k; i++) {
        Entry element;
        element.dist = 0;
        element.city = capitals[i];
        element.state = i + 1;
        heapPush(element);
    }

    int assigned = 0;
    while (assigned < totalCities && heapSize > 0) {
        Entry current = heapPop();
        int city = current.city;

        if (visited[city])
            continue;

        visited[city] = 1;
        belong[city] = current.state;

        int state = current.state;
        stateList[state][stateListSize[state]] = city;
        stateListSize[state]++;
        assigned++;

        // Перебираем всех соседей текущего города
        Edge* element = head[city];
        while (element != NULL) {
            int neigh = element->to;
            if (!visited[neigh]) {
                Entry next;
                next.dist = current.dist + element->len;
                next.city = neigh;
                next.state = current.state;
                heapPush(next);
            }
            element = element->next;
        }
    }
}

// Получение указателя на массив городов государства и их количества 
int* getStateCities(int state, int* count) {
    if (state < 1 || state > totalStates) {
        *count = 0;
        return NULL;
    }
    *count = stateListSize[state];
    return stateList[state];
}

// Освобождение памяти
void freeMemory(void)
{
    // Освобождаем рёбра
    for (int i = 1; i <= totalCities; i++) {
        Edge* element = head[i];
        while (element) {
            Edge* next = element->next;
            free(element);
            element = next;
        }
        head[i] = NULL; // обнуляем на всякий случай
    }

    // Освобождаем списки государств (только для существующих государств)
    for (int i = 1; i <= totalStates; i++) {
        if (stateList[i]) {
            free(stateList[i]);
            stateList[i] = NULL;
        }
    }

    // Освобождаем кучу
    free(heap);
    heap = NULL;
    heapSize = 0;
    heapCap = 0;
}