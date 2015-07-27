# cmake_minimum_required(VERSION 2.8)

# project(sqlparser)

set(BUILD_DIR "./build")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


file(GLOB sqlparser_src
        "${BUILD_DIR}/*.cpp"
        "${BUILD_DIR}/*.h"
)

add_library(sqlparser STATIC ${sqlparser_src})
