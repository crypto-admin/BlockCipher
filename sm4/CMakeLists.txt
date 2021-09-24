cmake_minimum_required(VERSION 3.16)
# project(sm4 C)

set(CMAKE_C_STANDARD 11)


file(GLOB_RECURSE SRC_FRONTEND ${CMAKE_SOURCE_DIR}/sm4/*.c)
add_executable(sm4 ${SRC_FRONTEND})
