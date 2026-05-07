# Создание статической библиотеки
add_library(hw_1_csv_lib STATIC hw_1_csv.c)

# Указание заголовочного файла
target_include_directories(hw_1_csv_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Создание исполняемого файла из main-файла
add_executable(hw_1_csv hw_1_csv_main.c)

# Расположение исполняемого файйла внутри проекта
set_target_properties(hw_1_csv PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/src/hw_1_csv/bin
)

# Связка исполняемого файла с библиотекой
target_link_libraries(hw_1_csv PRIVATE hw_1_csv_lib)
