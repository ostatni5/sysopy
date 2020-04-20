cmake_minimum_required(VERSION 3.10)
project(zad2 C)

set(CMAKE_C_STANDARD 11)

add_executable(generate generate.c)

add_executable(macierz macierz.c)

add_executable(validate validate.c)