cmake_minimum_required(VERSION 3.2)
project(MQCloud)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SOURCES
    "include/MQCloud/*"
    "include/MQCloud/Internal/*"
    "src/Internal/*"
    "src/*"
)

set(SOURCE_FILES
    external/ReadMe.txt
    ${SOURCES}
    )



include_directories(external)
include_directories(include)

add_executable(MQCloud ${SOURCE_FILES})