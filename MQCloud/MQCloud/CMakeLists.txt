cmake_minimum_required(VERSION 3.2)
project(MQCloud)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SOURCES
    "include/MQCloud/*"
    "include/MQCloud/Internal/*"
    "src/Internal/*"
    "src/*"
)

file(GLOB TEST_SOURCES
        "../MQCloudTest/*"
        )

set(SOURCE_FILES
    external/ReadMe.txt
    ${SOURCES}
    )

set(TEST_SOURCE_FILES
        external/ReadMe.txt
        ${TEST_SOURCES}
        )



include_directories(external)
include_directories(external/include)

add_library(MQCloud ${SOURCE_FILES})
add_executable(MQCloudTest ${TEST_SOURCE_FILES})