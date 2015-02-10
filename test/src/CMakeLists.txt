cmake_minimum_required(VERSION 2.8)
PROJECT(libkafka_test)

find_package(ZLIB REQUIRED)
find_package(Snappy REQUIRED)
find_package(GTest REQUIRED)

file(GLOB libkafka_test_HEADERS ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB libkafka_test_SOURCES ${PROJECT_SOURCE_DIR}/*.cc)

set(KAFKA_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../../lib/src")
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${KAFKA_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${SNAPPY_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS})

add_executable(kafka_test ${libkafka_test_HEADERS} ${libkafka_test_SOURCES})
set_property(TARGET kafka_test PROPERTY CXX_STANDARD 11)
target_link_libraries(kafka_test
    kafka 
    ${ZLIB_LIBRARIES}
    ${SNAPPY_LIBRARIES}
    ${GTEST_LIBRARIES})
GTEST_ADD_TESTS(kafka_test "" ${libkafka_test_SOURCES})
