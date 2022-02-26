cmake_minimum_required(VERSION 3.21)
project(A2 C)

set(CMAKE_C_STANDARD 11)



add_executable(
        A2
        main.c
        global_var.h
        input.c input.h
        output.c output.c
        send.c send.h
        receive.c receive.h
        soc.c soc.h)

#add_executable(
#        input
#        input.h input.c
#        list.h list.o
#        global_var.h
#)

#target_precompile_headers(
#        input
#        PUBLIC
#            list.h
#
#)