cmake_minimum_required(VERSION 3.16)
# project(aes C)

set(CMAKE_C_STANDARD 11)


file(GLOB_RECURSE SRC_FRONTEND ${CMAKE_SOURCE_DIR}/aes/*.c)
add_executable(aes ${SRC_FRONTEND})
