cmake_minimum_required(VERSION 2.8)

project(console)
execute_process(COMMAND pwd WORKING_DIRECTORY ./)

set(console_src ./main.cpp)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -std=c++11")
include_directories(../tarantool-connector/third_party/logger ../tarantool-connector/third_party/tarantool-c ../tarantool-connector/third_party/sqlparser/build/ ./ ../tarantool-connector/)
link_directories(../tarantool-connector/third_party/logger ../tarantool-connector/third_party/tarantool-c ../tarantool-connector/third_party/sqlparser/ ../tarantool-connector/)

add_executable(main ${console_src})

set_target_properties(main
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "bin"
)
add_dependencies(main sqlconn logger tpc sqlparser)
target_link_libraries(main sqlconn logger tpc sqlparser)
