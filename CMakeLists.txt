cmake_minimum_required(VERSION 3.15)
project(crawler C)

set(CMAKE_C_STANDARD 99)

add_executable(crawler src/main.c src/vector.c src/utils.c)