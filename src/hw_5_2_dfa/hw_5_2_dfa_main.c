#include "hw_5_2_dfa.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Переходы:
    Transition transitions[] = {
        { 0, 'd', 2 }, { 0, '.', 3 }, { 0, '-', 1 },
        { 1, 'd', 2 },
        { 2, 'd', 2 }, { 2, '.', 4 }, { 2, 'E', 6 },
        { 3, 'd', 5 },
        { 4, 'd', 5 },
        { 5, 'd', 5 }, { 5, 'E', 6 },
        { 6, 'd', 8 }, { 6, '+', 7 }, { 6, '-', 7 },
        { 7, 'd', 8 },
        { 8, 'd', 8 }
    };
    int transCount = sizeof(transitions) / sizeof(transitions[0]);

    int acceptingStates[] = { 2, 5, 8 };
    int acceptCount = sizeof(acceptingStates) / sizeof(acceptingStates[0]);

    DFA* dfa = dfaCreate(0, acceptingStates, acceptCount, transitions, transCount);
    if (!dfa) {
        printf("Ошибка создания ДКА\n");
        return 1;
    }

    printf("Введите строку для проверки: ");
    char buffer[256];
    if (scanf("%255s", buffer) != 1) {
        printf("Ошибка ввода\n");
        dfaFree(dfa);
        return 1;
    }

    int error;
    bool success = dfaAccept(dfa, buffer, &error);
    if (error == 1)
        printf("Это не число :(\n");
    if (error != 1 && success)
        printf("Это число!\n");
    if (error != 1 && !success)
        printf("Это не число :(\n");

    dfaFree(dfa);
    return 0;
}