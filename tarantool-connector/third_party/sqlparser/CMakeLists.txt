# cmake_minimum_required(VERSION 2.8)

# project(sqlparser)

execute_process(COMMAND make WORKING_DIRECTORY ./third_party/sqlparser/src)
execute_process(COMMAND make WORKING_DIRECTORY ./third_party/sqlparser/src/parser)

set(BUILD_DIR "./build")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB sqlparser_src
        "${BUILD_DIR}/*.cpp"
        "${BUILD_DIR}/*.h"
)

add_library(sqlparser STATIC ${sqlparser_src})
