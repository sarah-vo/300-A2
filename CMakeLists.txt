cmake_minimum_required(VERSION 3.21)
project(A2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-pthread")


add_executable(
        A2
        main.c
        global_var.h
        input.c input.h
        output.c output.c
        send.c send.h
        receive.c receive.h
        soc.c soc.h global_var.c main.h)

add_executable(
        input.c
        list.o

        global_var.c)

