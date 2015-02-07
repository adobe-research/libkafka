cmake_minimum_required(VERSION 2.8)
PROJECT(kafka)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/src")

option(KAFKA_TESTS "Enable the build of libkafka unit tests" ON)
if(KAFKA_TESTS)
    enable_testing()
    include(Dart)
    add_subdirectory("${PROJECT_SOURCE_DIR}/test/src")
endif()

