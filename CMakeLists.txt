cmake_minimum_required(VERSION 3.7)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -O0" )

set(SOURCE_FILES main.cpp)
add_executable(untitled ${SOURCE_FILES})