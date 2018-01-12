cmake_minimum_required(VERSION 3.9)
project(reverse C)

set(CMAKE_C_STANDARD 99)

add_executable(reverse main.c client.c server.c server.h client.h common.h)