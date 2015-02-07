cmake_minimum_required(VERSION 2.8)
PROJECT(kafka)

find_package(ZLIB REQUIRED)

file(GLOB libkafka_HEADERS
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/fetch/*.h
    ${PROJECT_SOURCE_DIR}/metadata/*.h
    ${PROJECT_SOURCE_DIR}/offset/*.h
    ${PROJECT_SOURCE_DIR}/produce/*.h
)

file(GLOB libkafka_SOURCES
    ${PROJECT_SOURCE_DIR}/*.cc
    ${PROJECT_SOURCE_DIR}/fetch/*.cc
    ${PROJECT_SOURCE_DIR}/metadata/*.cc
    ${PROJECT_SOURCE_DIR}/offset/*.cc
    ${PROJECT_SOURCE_DIR}/produce/*.cc
)

include_directories( ${PROJECT_SOURCE_DIR} ${ZLIB_INCLUDE_DIRS} )
add_library(kafka STATIC ${libkafka_HEADERS} ${libkafka_SOURCES})
set_property(TARGET kafka PROPERTY CXX_STANDARD 11)

