cmake_minimum_required(VERSION 3.7)
project(analyzer)
enable_testing()

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS system filesystem date_time unit_test_framework REQUIRED)

add_executable(analyzer main.cpp file_stats.h file_stats.cpp directory_tree.h directory_tree.cpp html_generator.cpp html_generator.h)

target_link_libraries(analyzer ${Boost_LIBRARIES})

add_executable(file_stats_tests file_stats.h file_stats.cpp ./tests/test_file_stats.cpp)
target_link_libraries(file_stats_tests ${Boost_LIBRARIES})
add_test(NAME file_stats_tests COMMAND file_stats_tests)

add_executable(directory_tree_tests directory_tree.h directory_tree.cpp file_stats.h file_stats.cpp ./tests/test_directory_tree.cpp)
target_link_libraries(directory_tree_tests ${Boost_LIBRARIES})
add_test(NAME directory_tree_tests COMMAND directory_tree_tests)