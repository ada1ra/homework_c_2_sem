#include "hw_5_2_dfa.h"
#include <assert.h>
#include <stdio.h>

int main()
{
    // Таблица переходов (из main)
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

    // Проверка создания ДКА
    assert(dfa != NULL);

    // Проверки корректных чисел
    assert(dfaAccept(dfa, "38.871E5", NULL) == true);
    assert(dfaAccept(dfa, ".591", NULL) == true);
    assert(dfaAccept(dfa, "123", NULL) == true);
    assert(dfaAccept(dfa, "-5.6E+2", NULL) == true);
    assert(dfaAccept(dfa, "0", NULL) == true);
    assert(dfaAccept(dfa, "-0.1E-2", NULL) == true);
    assert(dfaAccept(dfa, "5E10", NULL) == true);

    // Проверки некорректных чисел
    assert(dfaAccept(dfa, "А я число?", NULL) == false);
    assert(dfaAccept(dfa, "823.16.10", NULL) == false);
    assert(dfaAccept(dfa, "abc", NULL) == false);
    assert(dfaAccept(dfa, "123.", NULL) == false);
    assert(dfaAccept(dfa, "", NULL) == false);
    assert(dfaAccept(dfa, "E10", NULL) == false);
    assert(dfaAccept(dfa, "--5", NULL) == false);
    assert(dfaAccept(dfa, ".", NULL) == false);
    assert(dfaAccept(dfa, "1E", NULL) == false);
    assert(dfaAccept(dfa, "1E+", NULL) == false);

    dfaFree(dfa);
    printf("All tests passed!\n");
    return 0;
}