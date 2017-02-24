cmake_minimum_required(VERSION 2.6)
project(SPMAT)

#set(CMAKE_C_FLAGS "-Wall -msse2 -march=native")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)
