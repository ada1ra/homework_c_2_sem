#pragma once

#include <stdio.h>

// Строка таблицы
typedef struct Row Row;

// Разбор всего CSV-файла
Row* csvReading(const char* filename, int* outRowsCount, int* outColsCount);

// Вычисление максимальной ширины столбца
int* maxColWidthFinding(const Row* rows, int rowsCount, int colsCount);

// Печать всей таблицы
void printTable(const Row* rows, int rowsCount, int colsCount, const int* maxWidth, FILE* out);

// Освобождение памяти
void freeRows(Row* rows, int rowsCount, int colsCount);