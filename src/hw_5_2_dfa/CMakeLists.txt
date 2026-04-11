# Создание статической библиотеки
add_library(hw_5_2_dfa STATIC hw_5_2_dfa.c)

# Указание заголовочного файла
target_include_directories(hw_5_2_dfa_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Создание исполняемого файла из main-файла
add_executable(hw_5_2_dfa hw_5_2_dfa_main.c)

# Связка исполняемого файла с библиотекой
target_link_libraries(hw_5_2_dfa PRIVATE hw_5_2_dfa_lib)