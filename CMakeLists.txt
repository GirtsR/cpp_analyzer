cmake_minimum_required(VERSION 3.9)
project(analyzer)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS system filesystem date_time REQUIRED)

add_executable(analyzer main.cpp file_stats.h file_stats.cpp directory_tree.h directory_tree.cpp)

target_link_libraries(analyzer ${Boost_LIBRARIES})