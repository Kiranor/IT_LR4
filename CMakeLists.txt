cmake_minimum_required(VERSION 3.19)
project(IT_LR4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
add_executable(IT_LR4 main.cpp)