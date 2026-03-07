#include "hw_1_csv.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_BUFFER 10000

// Поле таблицы
typedef struct {
    char* str;
    int isNumber;
} Field;

// Строка таблицы
struct Row {
    Field* fields;
    int fieldsCount;
};

// Проверка, является ли строка числом
static int isNumber(const char* s)
{
    if (*s == '-' || *s == '+')
        s++;
    if (!*s)
        return 0;
    int dot = 0;
    while (*s) {
        s++;
        if (*s == '.') {
            if (dot)
                return 0;
            dot = 1;
        }
        if (!isdigit(*s))
            return 0;
    }
    return 1;
}

// Определение количества столбцов по первой строке
static int columnCounting(const char* line)
{
    int cols = 1;
    for (const char* p = line; *p; p++) {
        if (*p == ',')
            cols++;
    }
    return cols;
}

// Разбор одной строки
static void lineParsing(const char* line, Field* fields, int colsCount)
{
    char* token = (char*)line;
    char* next;
    int col = 0;
    while (token && col < colsCount) {
        next = strchr(token, ',');
        if (next)
            *next = '\0';
        fields[col].str = (char*)malloc(strlen(token) + 1);
        strcpy(fields[col].str, token);
        fields[col].isNumber = isNumber(fields[col].str);
        col++;
        if (next)
            token = next + 1;
        else
            break;
    }
}

// Разбор всего CSV-файла
Row* csvReading(const char* filename, int* outRowsCount, int* outColsCount)
{
    FILE* inputFile = fopen(filename, "r");
    if (!inputFile) {
        printf("Can't open input.csv\n");
        return NULL;
    }

    Row* rows = NULL;
    char line[MAX_LINE];
    int rowsCount = 0;
    int colsCount = -1;

    while (fgets(line, sizeof(line), inputFile)) {
        size_t len = strlen(line);
        if (len && line[len - 1] == '\n')
            line[len - 1] = '\0';

        if (colsCount == -1)
            colsCount = columnCounting(line);

        Field* fields = (Field*)malloc(colsCount * sizeof(Field));
        for (int i = 0; i < colsCount; i++)
            fields[i].str = NULL;

        lineParsing(line, fields, colsCount);

        rows = (Row*)realloc(rows, (rowsCount + 1) * sizeof(Row));
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
int* maxColWidthFinding(const Row* rows, int rowsCount, int colsCount)
{
    int* maxWidth = (int*)malloc(colsCount * sizeof(int));
    for (int j = 0; j < colsCount; j++) {
        maxWidth[j] = 0;
        for (int i = 0; i < rowsCount; i++) {
            int len = strlen(rows[i].fields[j].str);
            if (len > maxWidth[j])
                maxWidth[j] = len;
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
    char* p;

    // Заголовок
    p = buffer;
    *p++ = '|';
    for (int j = 0; j < colsCount; j++) {
        p += sprintf(p, " %-*s |", maxWidth[j], rows[0].fields[j].str);
    }
    *p++ = '\n';
    *p = '\0';
    fputs(buffer, out);

    // Линия после заголовка
    printLine(out, colsCount, maxWidth, '=');

    // Данные
    for (int i = 1; i < rowsCount; i++) {
        p = buffer;
        *p++ = '|';
        for (int j = 0; j < colsCount; j++) {
            if (rows[i].fields[j].isNumber) {
                p += sprintf(p, " %*s |", maxWidth[j], rows[i].fields[j].str);
            } else {
                p += sprintf(p, " %-*s |", maxWidth[j], rows[i].fields[j].str);
            }
        }
        *p++ = '\n';
        *p = '\0';
        fputs(buffer, out);

        // Разделитель строк данных
        printLine(out, colsCount, maxWidth, '-');
    }
}

// Освобождение памяти
void freeRows(Row* rows, int rowsCount, int colsCount)
{
    for (int i = 0; i < rowsCount; i++) {
        for (int j = 0; j < colsCount; j++)
            free(rows[i].fields[j].str);
        free(rows[i].fields);
    }
    free(rows);
}