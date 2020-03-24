cmake_minimum_required(VERSION 3.10.2)
project(crawler C)
enable_language(C CXX ASM)
set(CMAKE_C_FLAGS  "-std=c11")


#
## Source definitions ###
#

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB sources
        "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(crawler ${sources})