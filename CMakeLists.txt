cmake_minimum_required(VERSION 3.9)
project(analyzer)

set(CMAKE_CXX_STANDARD 17)

add_executable(analyzer main.cpp file_stats.h file_stats.cpp)