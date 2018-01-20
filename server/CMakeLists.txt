cmake_minimum_required(VERSION 3.7)
project(server)

set(CMAKE_C_STANDARD 99)

set(server_SOURCE_FILES
        main.c)

add_executable(server ${server_SOURCE_FILES})