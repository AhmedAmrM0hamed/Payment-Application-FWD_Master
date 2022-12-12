cmake_minimum_required(VERSION 3.23)
project(payment C)

set(CMAKE_C_STANDARD 99)

add_executable(payment main.c
        Application/app.c
        Application/app.h
        Server/server.h
        Terminal/terminal.h
        Card/card.h
        Application/app.c
        Server/server.c
        Terminal/terminal.c
        Card/card.c
        "Data Types/dataTypes.h")
