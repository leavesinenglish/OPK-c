cmake_minimum_required(VERSION 3.17)
project(test_console C)

set(CMAKE_C_STANDARD 11)

add_executable(test_console main.c console.c console.h test_field_console.c packman.c packman.h)