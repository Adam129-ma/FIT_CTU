cmake_minimum_required(VERSION 3.22)
project(Velkauloha)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -pedantic -g)
endif()

add_executable(Velkauloha test.cpp)

find_library(EXPRESSION_PARSER_LIB expression_parser PATHS ./i686-w64-mingw32)

target_link_libraries(Velkauloha ${EXPRESSION_PARSER_LIB})