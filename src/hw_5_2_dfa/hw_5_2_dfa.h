#pragma once
#include <stdbool.h>

// Структура для описания перехода
typedef struct {
    int fromState;
    char symbol;
    int toState;
} Transition;

// Структура ДКА
typedef struct DFA DFA;

// Создание ДКА
DFA* dfaCreate(int startState, const int* acceptingStates, int acceptingCount,
    const Transition* transitions, int transitionCount);

// Проверка строки
bool dfaAccept(const DFA* dfa, const char* str, int* errorCode);

// Освобождение памяти
void dfaFree(DFA* dfa);
