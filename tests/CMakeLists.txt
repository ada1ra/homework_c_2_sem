# Исполняемый файл для тестирования
add_executable(hw_5_2_dfa_test hw_5_2_dfa_test.c)

# Линкуем с библиотекой
target_link_libraries(hw_5_2_dfa_test PRIVATE hw_5_2_dfa_lib)

# Добавляем тест в CTest
add_test(NAME DFA_Test COMMAND hw_5_2_dfa_test)
