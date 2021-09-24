cmake_minimum_required(VERSION 3.16)
project(BlockCipher C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(sha1)
add_subdirectory(sm3)
add_subdirectory(aes)
add_subdirectory(sm4)


