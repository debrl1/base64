cmake_minimum_required(VERSION 3.21)
project(b64 C)

set(CMAKE_C_STANDARD 99)

add_library(b64 base64.c base64.h)

add_executable(b64_exec main.c base64.c base64.h)
