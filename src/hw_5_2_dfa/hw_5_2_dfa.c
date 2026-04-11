#include "hw_5_2_dfa.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Структура ДКА
struct DFA {
    int startState;
    int* acceptingStates;
    int acceptingCount;
    Transition* transitions;
    int transitionCount;
};

// Служебная функция: классифицирует символ
static char classifyChar(char c)
{
    unsigned char uc = (unsigned char)c;
    if (isdigit(uc))
        return 'd';
    else {
        const char* allowed = ".E+-";
        if (strchr(allowed, uc))
            return c;
    }
    return 0;
}

// Создание ДКА
DFA* dfaCreate(int startState, const int* acceptingStates, int acceptingCount,
    const Transition* transitions, int transitionCount)
{
    DFA* dfa = (DFA*)malloc(sizeof(DFA));
    if (!dfa)
        return NULL;
    dfa->startState = startState;
    dfa->acceptingCount = acceptingCount;
    dfa->acceptingStates = (int*)malloc(acceptingCount * sizeof(int));
    if (!dfa->acceptingStates) {
        free(dfa);
        return NULL;
    }
    memcpy(dfa->acceptingStates, acceptingStates, acceptingCount * sizeof(int));

    dfa->transitionCount = transitionCount;
    dfa->transitions = (Transition*)malloc(transitionCount * sizeof(Transition));
    if (!dfa->transitions) {
        free(dfa->acceptingStates);
        free(dfa);
        return NULL;
    }
    memcpy(dfa->transitions, transitions, transitionCount * sizeof(Transition));
    return dfa;
}

// Проверка строки
bool dfaAccept(const DFA* dfa, const char* str, int* errorCode)
{
    int state = dfa->startState;
    for (const char* p = str; *p; p++) {
        char sym = classifyChar(*p);
        if (sym == 0) {
            if (errorCode)
                *errorCode = 1;
            return false;
        }

        int next = -1;
        for (int i = 0; i < dfa->transitionCount; i++) {
            if (dfa->transitions[i].fromState == state && dfa->transitions[i].symbol == sym) {
                next = dfa->transitions[i].toState;
                break;
            }
        }
        if (next == -1) {
            if (errorCode)
                *errorCode = 2;
            return false;
        }
        state = next;
    }
    for (int i = 0; i < dfa->acceptingCount; i++) {
        if (dfa->acceptingStates[i] == state) {
            if (errorCode)
                *errorCode = 0;
            return true;
        }
    }
    if (errorCode)
        *errorCode = 2;
    return false;
}

// Освобождение памяти
void dfaFree(DFA* dfa)
{
    if (dfa) {
        free(dfa->acceptingStates);
        free(dfa->transitions);
        free(dfa);
    }
}
