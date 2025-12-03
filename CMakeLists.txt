cmake_minimum_required(VERSION 3.27)
project(sus C)

set(CMAKE_C_STANDARD 11)

add_executable(sus main.c
        server.c
        server.h
        http/requests.c
        http/requests.h
        unit_tests.c)

add_executable(unit_testing
        http/requests.c
        unit_tests.c)
