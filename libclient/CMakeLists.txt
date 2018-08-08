cmake_minimum_required(VERSION 3.7)
project(cashka_libclient)

set(CMAKE_C_STANDARD 11)

add_library(libcashka SHARED cashka.h cashka.c)