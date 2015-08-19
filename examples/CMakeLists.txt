cmake_minimum_required(VERSION 3.2)

include_directories ("${PROJECT_SOURCE_DIR}/src")

add_executable(demo demo.c)

target_link_libraries (demo iso8583)

add_executable(benchmark benchmark.c)

target_link_libraries (benchmark iso8583)
