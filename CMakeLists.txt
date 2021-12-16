cmake_minimum_required(VERSION 3.21)
project(b64 C)

set(CMAKE_C_STANDARD 99)

add_library(b64 include/base64.h src/digits.c src/digits.h src/encoding.c src/decoding.c)

add_executable(b64_exec include/base64.h src/digits.c src/digits.h src/encoding.c src/decoding.c examples/encoding.c)
