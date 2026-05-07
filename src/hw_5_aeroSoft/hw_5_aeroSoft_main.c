#include "hw_5_aeroSoft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Пропуск пробелов в строке
static char* skipSpaces(char* s)
{
    while (*s == ' ')
        s++;
    return s;
}

// Загрузка базы из файла
static int loadAirports(Tree* tree, const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    char line[512];
    int loaded = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        char* colon = strchr(line, ':');
        if (!colon)
            continue;
        *colon = '\0';
        if (!(treeInsert(tree, line, colon + 1)))
            printf("Не удалось добавить аэропорт с кодом %s.\n", line);
        loaded++;
    }
    fclose(file);
    return loaded;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <airports.txt>\n", argv[0]);
        return 1;
    }

    Tree* tree = createTree();
    if (!tree) {
        fprintf(stderr, "Ошибка создания дерева\n");
        return 1;
    }

    int loaded = loadAirports(tree, argv[1]);
    if (loaded < 0) {
        perror("Не удалось открыть файл");
        treeFree(tree);
        return 1;
    }
    printf("Загружено %d аэропортов. Система готова к работе.\n", loaded);

    char cmd[16];
    char line[512];
    while (true) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin))
            break;
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue;

        char* p = skipSpaces(line);
        char* cmdEnd = p;
        while (*cmdEnd && *cmdEnd != ' ')
            cmdEnd++;
        size_t cmdLen = (size_t)(cmdEnd - p);
        if (cmdLen >= sizeof(cmd))
            cmdLen = sizeof(cmd) - 1;
        strncpy(cmd, p, cmdLen);
        cmd[cmdLen] = '\0';
        char* argStart = skipSpaces(cmdEnd);

        if (strcmp(cmd, "quit") == 0) {
            break;
        } else if (strcmp(cmd, "find") == 0) {
            if (strlen(argStart) == 0) {
                printf("Не указан код аэропорта.\n");
                continue;
            }
            const char* name = treeSearch(tree, argStart);
            if (name) {
                printf("%s → %s\n", argStart, name);
            } else {
                printf("Аэропорт с кодом '%s' не найден в базе.\n", argStart);
            }
        } else if (strcmp(cmd, "add") == 0) {
            char* colon = strchr(argStart, ':');
            if (!colon) {
                printf("Формат: add код:название\n");
                continue;
            }
            *colon = '\0';
            char* code = argStart;
            char* name = colon + 1;
            const char* existing = treeSearch(tree, code);
            if (existing) {
                printf("Аэропорт '%s' уже существует.\n", code);
                continue;
            }
            if (!(treeInsert(tree, code, name)))
                printf("Не удалось добавить аэропорт с кодом %s.\n", line);

            printf("Аэропорт '%s' добавлен в базу.\n", code);
        } else if (strcmp(cmd, "delete") == 0) {
            if (strlen(argStart) == 0) {
                printf("Не указан код аэропорта.\n");
                continue;
            }
            const char* existing = treeSearch(tree, argStart);
            if (!existing) {
                printf("Аэропорт '%s' не найден.\n", argStart);
                continue;
            }
            treeRemove(tree, argStart);
            printf("Аэропорт '%s' удалён из базы.\n", argStart);
        } else if (strcmp(cmd, "save") == 0) {
            FILE* out = fopen(argv[1], "w");
            if (!out) {
                perror("Ошибка сохранения");
                continue;
            }
            treeSave(tree, out);
            fclose(out);
            printf("База сохранена.\n");
        } else
            printf("Неизвестная команда. Доступны: find, add, delete, save, quit\n");
    }

    treeFree(tree);
    return 0;
}
