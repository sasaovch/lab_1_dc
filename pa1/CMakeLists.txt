cmake_minimum_required(VERSION 3.23)
project(lab_1_pa C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

add_executable(lab1 common.h io.c io.h ipc.h  main.c structures.h structures.c pa1.h pipes.c pipes.h threads.h threads.c)
