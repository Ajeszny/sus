cmake_minimum_required(VERSION 3.27)
project(sus C)

set(CMAKE_C_STANDARD 11)

add_executable(sus main.c
        server.c
        server.h
        http/requests.c
        http/requests.h
        static.c
        static.h
        http/response.c
        http/response.h
        mime/types.c
        mime/types.h
)

add_executable(unit_testing
        http/requests.c
        unit_tests.c
        unit_tests.c
        unit_tests.c
        mime/types.c
        mime/types.h)
target_link_libraries(sus ws2_32)
target_link_libraries(unit_testing ws2_32)
