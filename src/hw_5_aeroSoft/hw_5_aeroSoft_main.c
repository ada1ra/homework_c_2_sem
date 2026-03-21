#include "hw_5_aeroSoft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Вспомогательная функция для сохранения дерева в файл
static void saveNode(Node* node, FILE* f)
{
    if (!node)
        return;
    saveNode(node->left, f);
    fprintf(f, "%s:%s\n", node->code, node->name);
    saveNode(node->right, f);
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

    // Загрузка базы
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("Не удалось открыть файл");
        treeFree(tree);
        return 1;
    }

    char line[512];
    int loaded = 0;
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';
        char* colon = strchr(line, ':');
        if (!colon)
            continue;
        *colon = '\0';
        treeInsert(tree, line, colon + 1);
        loaded++;
    }
    fclose(f);
    printf("Загружено %d аэропортов. Система готова к работе.\n", loaded);

    char cmd[16], arg[256];
    while (1) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin))
            break;
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue;

        char cmd[16];
        char* arg = line;
        // пропускаем пробелы
        while (*arg == ' ')
            arg++;
        // извлекаем команду
        char* cmdEnd = arg;
        while (*cmdEnd && *cmdEnd != ' ')
            cmdEnd++;
        ptrdiff_t cmdLen = cmdEnd - arg;
        if (cmdLen >= (ptrdiff_t)sizeof(cmd))
            cmdLen = sizeof(cmd) - 1;
        strncpy(cmd, arg, (size_t)cmdLen);
        cmd[cmdLen] = '\0';
        // аргумент - остаток строки
        arg = cmdEnd;
        while (*arg == ' ')
            arg++;
        while (*arg == ' ')
            arg++;

        if (strcmp(cmd, "quit") == 0)
            break;
        else if (strcmp(cmd, "find") == 0) {
            if (strlen(arg) == 0) {
                printf("Не указан код аэропорта.\n");
                continue;
            }
            char* res = treeSearch(tree, arg);
            if (strstr(res, "не найден")) {
                printf("%s\n", res);
            } else
                printf("%s → %s\n", arg, res);
            free(res);
        } else if (strcmp(cmd, "add") == 0) {
            char* colon = strchr(arg, ':');
            if (!colon) {
                printf("Формат: add код:название\n");
                continue;
            }
            *colon = '\0';
            char* code = arg;
            char* name = colon + 1;
            // проверка существования
            char* check = treeSearch(tree, code);
            if (strstr(check, "не найден") == NULL) {
                printf("Аэропорт '%s' уже существует.\n", code);
                free(check);
                continue;
            }
            free(check);
            treeInsert(tree, code, name);
            printf("Аэропорт '%s' добавлен в базу.\n", code);
        } else if (strcmp(cmd, "delete") == 0) {
            if (strlen(arg) == 0) {
                printf("Не указан код аэропорта.\n");
                continue;
            }
            char* check = treeSearch(tree, arg);
            if (!check) {
                printf("Ошибка памяти.\n");
                continue;
            }
            if (strstr(check, "не найден")) {
                printf("Аэропорт '%s' не найден.\n", arg);
                free(check);
                continue;
            }
            free(check);
            treeRemove(tree, arg);
            printf("Аэропорт '%s' удалён из базы.\n", arg);
        } else if (strcmp(cmd, "save") == 0) {
            FILE* out = fopen(argv[1], "w");
            if (!out) {
                perror("Ошибка сохранения");
                continue;
            }
            saveNode(tree->root, out);
            fclose(out);
            printf("База сохранена.\n");
        } else
            printf("Неизвестная команда. Доступны: find, add, delete, save, print, quit\n");
    }

    treeFree(tree);
    return 0;
}
