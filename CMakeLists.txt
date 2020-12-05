cmake_minimum_required(VERSION 3.16)
project(ping C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(ping
        include/ping.h
        src/main.c)
