cmake_minimum_required(VERSION 3.16)
# project(sha1 C)
find_package(SEAL 3.6 REQUIRED)
set(CMAKE_CXX_STANDARD 11)
project(sealpir_test VERSION 2.2 LANGUAGES CXX)


file(GLOB_RECURSE SRC_FRONTEND ${CMAKE_SOURCE_DIR}/sealpir_test/*.cpp)
add_executable(sealpir_test ${SRC_FRONTEND})
target_link_libraries(sealpir_test SEAL::seal)
