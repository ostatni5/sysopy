cmake_minimum_required(VERSION 3.10)
project(zad4 C)

set(CMAKE_C_STANDARD 11)

add_executable(sender sender.c)
add_executable(catcher catcher.c)