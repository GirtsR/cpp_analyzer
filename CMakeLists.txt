cmake_minimum_required(VERSION 3.7)
project(analyzer)
enable_testing()

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_executable(analyzer main.cpp file_stats.h file_stats.cpp directory_tree.h directory_tree.cpp html_generator.cpp html_generator.h)
include_directories(analyzer PRIVATE ${Boost_INCLUDE_DIR})

target_link_libraries(analyzer ${Boost_LIBRARIES})

add_executable(unit_tests file_stats.h file_stats.cpp tests/test_unit.cpp)
target_link_libraries(unit_tests ${Boost_LIBRARIES})
add_test(NAME unit_tests COMMAND unit_tests)

add_executable(performance_tests directory_tree.h directory_tree.cpp file_stats.h file_stats.cpp tests/test_performance.cpp)
target_link_libraries(performance_tests ${Boost_LIBRARIES})
add_test(NAME performance_tests COMMAND performance_tests)

add_executable(integration_tests directory_tree.h directory_tree.cpp file_stats.h file_stats.cpp tests/test_integration.cpp)
target_link_libraries(integration_tests ${Boost_LIBRARIES})
add_test(NAME integration_tests COMMAND integration_tests)
