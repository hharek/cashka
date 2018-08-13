cmake_minimum_required(VERSION 3.7)
project(cashka_server)

set(CMAKE_C_STANDARD 11)

add_executable(cashka
                    inc/cashka.h
                    inc/err.h
                    inc/err.c
                    opt/opt.h
                    opt/opt.c
                    opt/cli.c
                    opt/cfg.c
                    opt/def.c
                    main.c)