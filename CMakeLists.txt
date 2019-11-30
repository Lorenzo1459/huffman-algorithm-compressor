cmake_minimum_required(VERSION 3.15)
project(huffmano C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(huffmano
        bitmap.c
        bitmap.h
        compacta.c
        compactador.c
        compactador.h)
