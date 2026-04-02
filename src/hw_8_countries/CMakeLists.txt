# Создание статической библиотеки
add_library(hw_8_countries_lib STATIC hw_8_countries.c)

# Указание заголовочного файла
target_include_directories(hw_8_countries_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Создание исполняемого файла из main-файла
add_executable(hw_8_countries hw_8_countries_main.c)

# Связка исполняемого файла с библиотекой
target_link_libraries(hw_8_countries PRIVATE hw_8_countries_lib)
