cmake_minimum_required(VERSION 3.21)
project(sigprintf C)

set(CMAKE_C_STANDARD 90)

add_library(sigprintf STATIC sigprintf.c
	sigstring.c
	sigprintf_conv.c)

add_library(sigprintf_test SHARED sigprintf.c
	sigstring.c
	sigprintf_conv.c)

add_executable(sigprintf_driver sigprintf_driver.c)
target_link_libraries(sigprintf_driver sigprintf)

target_compile_options(sigprintf_test PRIVATE -DUNIT_TEST -fPIC)