#include "hw_1_csv.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_BUFFER 10000

// Поле таблицы
typedef struct {
    char* str;
    bool isNumber;
} Field;

// Строка таблицы
struct Row {
    Field* fields;
    int fieldsCount;
};

// Проверка, является ли строка числом
static bool isNumber(const char* string)
{
    if (*string == '-' || *string == '+')
        string++;
    if (!*string)
        return false;
    bool dot = false;
    while (*string) {
        if (*string == '.') {
            if (dot)
                return false;
            dot = true;
        }
        if (!isdigit(*string))
            return false;
    }
    string++;
    return true;
}

// Определение количества столбцов по первой строке
static int countColumns(const char* line)
{
    int cols = 1;
    for (const char* cursor = line; *cursor; cursor++) {
        if (*cursor == ',')
            cols++;
    }
    return cols;
}

// Безопасное копирование строки с выделением памяти
static char* safeStrdup(const char* src)
{
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char* dst = (char*)malloc(len + 1);
    if (dst) {
        memcpy(dst, src, len + 1);
    }
    return dst;
}

// Разбор одной строки
static bool parseLine(const char* line, Field* fields, int colsCount)
{
    char* lineCopy = strdup(line);
    if (!lineCopy)
        return false;

    char* token = lineCopy;
    int col = 0;
    while (token && col < colsCount) {
        char* next = strchr(token, ',');
        if (next)
            *next = '\0';
        fields[col].str = safeStrdup(token);
        if (!fields[col].str) {
            for (int i = 0; i < col; i++)
                free(fields[i].str);
            free(lineCopy);
            return false;
        }
        fields[col].isNumber = isNumber(fields[col].str);
        col++;
        if (next)
            token = next + 1;
        else
            break;
    }
    free(lineCopy);
    return true;
}

// Освобождение памяти
void freeRows(Row* rows, int rowsCount, int colsCount)
{
    if (!rows)
        return;
    for (int i = 0; i < rowsCount; i++) {
        for (int j = 0; j < colsCount; j++)
            free(rows[i].fields[j].str);
        free(rows[i].fields);
    }
    free(rows);
}

// Разбор всего CSV-файла
Row* readCSV(const char* filename, int* outRowsCount, int* outColsCount)
{
    FILE* inputFile = fopen(filename, "r");
    if (!inputFile)
        return NULL;

    Row* rows = NULL;
    char line[MAX_LINE];
    int rowsCount = 0;
    int colsCount = -1;

    while (fgets(line, sizeof(line), inputFile)) {
        size_t len = strlen(line);
        if (len && line[len - 1] == '\n')
            line[len - 1] = '\0';

        if (colsCount == -1)
            colsCount = countColumns(line);

        Field* fields = (Field*)malloc(colsCount * sizeof(Field));
        if (!fields)
            break;
        for (int i = 0; i < colsCount; i++)
            fields[i].str = NULL;

        if (!parseLine(line, fields, colsCount)) {
            free(fields);
            break;
        }

        // Безопасный realloc
        Row* newRows = (Row*)realloc(rows, (rowsCount + 1) * sizeof(Row));
        if (!newRows) {
            for (int i = 0; i < colsCount; i++)
                free(fields[i].str);
            free(fields);
            freeRows(rows, rowsCount, colsCount);
            rows = NULL;
            break;
        }
        rows = newRows;
        rows[rowsCount].fields = fields;
        rows[rowsCount].fieldsCount = colsCount;
        rowsCount++;
    }
    fclose(inputFile);

    *outRowsCount = rowsCount;
    *outColsCount = colsCount;
    return rows;
}

// Вычисление максимальной ширины столбца
int* findMaxColWidth(const Row* rows, int rowsCount, int colsCount)
{
    int* maxWidth = (int*)malloc(colsCount * sizeof(int));
    if (!maxWidth)
        return NULL;
    for (int j = 0; j < colsCount; j++) {
        maxWidth[j] = 0;
        for (int i = 0; i < rowsCount; i++) {
            size_t len = strlen(rows[i].fields[j].str);
            if (len > (size_t)maxWidth[j])
                maxWidth[j] = (int)len;
        }
    }
    return maxWidth;
}

// Печать горизонтальной линии
static void printLine(FILE* out, int colsCount, const int* maxWidth, char ch)
{
    fprintf(out, "+");
    for (int j = 0; j < colsCount; j++) {
        for (int k = 0; k < maxWidth[j] + 2; k++)
            fputc(ch, out);
        fputc('+', out);
    }
    fputc('\n', out);
}

// Печать всей таблицы
void printTable(const Row* rows, int rowsCount, int colsCount, const int* maxWidth, FILE* out)
{
    // Верхняя граница
    printLine(out, colsCount, maxWidth, '=');

    // Буфер для формирования строки таблицы
    char buffer[MAX_BUFFER];
    char* cursor = buffer;
    *cursor++ = '|';
    for (int j = 0; j < colsCount; j++)
        cursor += sprintf(cursor, " %-*s |", maxWidth[j], rows[0].fields[j].str);
    *cursor++ = '\n';
    *cursor = '\0';
    fputs(buffer, out);

    // Линия после заголовка
    printLine(out, colsCount, maxWidth, '=');

    // Данные
    for (int i = 1; i < rowsCount; i++) {
        cursor = buffer;
        *cursor++ = '|';
        for (int j = 0; j < colsCount; j++) {
            if (rows[i].fields[j].isNumber)
                cursor += sprintf(cursor, " %*s |", maxWidth[j], rows[i].fields[j].str);
            else
                cursor += sprintf(cursor, " %-*s |", maxWidth[j], rows[i].fields[j].str);
        }
        *cursor++ = '\n';
        *cursor = '\0';
        fputs(buffer, out);

        // Разделитель строк данных
        printLine(out, colsCount, maxWidth, '-');
    }
}
