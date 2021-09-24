cmake_minimum_required(VERSION 3.16)
# project(sm3 C)

set(CMAKE_C_STANDARD 11)


file(GLOB_RECURSE SRC_FRONTEND ${CMAKE_SOURCE_DIR}/sm3/*.c)
add_executable(sm3 ${SRC_FRONTEND})
