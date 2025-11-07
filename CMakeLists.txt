cmake_minimum_required(VERSION 3.31)
project(garutil C)

set(CMAKE_C_STANDARD 11)

option(GUTIL_TEST "Enable GUTIL testing mode" OFF)

add_library(gutil STATIC util/arena/arena.c
        util/debug/debug.c
        util/debug/debug.h)
target_include_directories(gutil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/util)

if(GUTIL_TEST)
    add_compile_definitions(GUTIL_TEST)
    add_executable(gutil_tests
            tests/rktest.c
            tests/arena_tests.c
    )
    target_link_libraries(gutil_tests PRIVATE gutil m)
    target_compile_definitions(gutil PRIVATE GUTIL_TEST)
    target_compile_definitions(gutil_tests PRIVATE GUTIL_TEST)
    target_include_directories(gutil PRIVATE util)
endif()
