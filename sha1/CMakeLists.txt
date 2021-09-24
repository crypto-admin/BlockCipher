cmake_minimum_required(VERSION 3.16)
# project(sha1 C)

set(CMAKE_C_STANDARD 11)


file(GLOB_RECURSE SRC_FRONTEND ${CMAKE_SOURCE_DIR}/sha1/*.c)
add_executable(sha1 ${SRC_FRONTEND})
