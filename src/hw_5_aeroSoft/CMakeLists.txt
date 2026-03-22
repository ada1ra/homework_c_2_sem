# Создание статической библиотеки
add_library(hw_5_aeroSoft_lib STATIC hw_5_aeroSoft.c)

# Указание заголовочного файла
target_include_directories(hw_5_aeroSoft_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Создание исполняемого файла из main-файла
add_executable(hw_5_aeroSoft hw_5_aeroSoft_main.c)

# Расположение исполняемого файйла внутри проекта
set_target_properties(hw_5_aeroSoft PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/src/hw_5_aeroSoft/bin
)

# Связка исполняемого файла с библиотекой
target_link_libraries(hw_5_aeroSoft PRIVATE hw_5_aeroSoft_lib)
