cmake_minimum_required(VERSION 3.1)
project(crawler C)
enable_language(C CXX ASM)
set(CMAKE_C_FLAGS  "-std=c11")

configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

enable_testing()
add_subdirectory(test)

#
## Source definitions ###
#

include_directories("${PROJECT_SOURCE_DIR}/include")
add_executable(${PROJECT_NAME} src/main.c src/list.c)

add_library(static_lib STATIC ${PROJECT_SOURCE_DIR}/src/utils.c)
add_library(shared_lib SHARED ${PROJECT_SOURCE_DIR}/src/utils.c)

target_link_libraries(${PROJECT_NAME} static_lib shared_lib)


