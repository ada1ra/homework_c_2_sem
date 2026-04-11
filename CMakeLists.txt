cmake_minimum_required(VERSION 3.28)

project(MultiProject C)
enable_testing()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(src/hw_5_2_dfa)

add_subdirectory(tests)
