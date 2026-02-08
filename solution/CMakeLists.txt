cmake_minimum_required(VERSION 3.31)
project(solution)

set(CMAKE_CXX_STANDARD 20)

add_executable(solution main.cpp
        random_stream_gen.h
        "hash_func_gen.h"
        hyperloglog.h
        tester.h
        bits_finder.cpp)
