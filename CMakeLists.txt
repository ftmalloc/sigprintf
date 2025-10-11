cmake_minimum_required(VERSION 3.21)
project(sigprintf C)

set(CMAKE_C_STANDARD 90)

add_library(sigprintf STATIC sigprintf.c)

add_executable(sigprintf_test sigprintf_test.c)
target_link_libraries(sigprintf_test sigprintf)
