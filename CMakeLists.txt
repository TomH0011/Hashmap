cmake_minimum_required(VERSION 4.0)
project(hash_map C)

set(CMAKE_C_STANDARD 11)

add_executable(hash_map hash_table.c
        hash_table.h
        prime.h
)
