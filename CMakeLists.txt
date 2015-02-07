cmake_minimum_required(VERSION 2.8)
PROJECT(kafka)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
enable_testing()

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/test/src")

