cmake_minimum_required(VERSION 2.8)

project(sqlconn)

set(BUILD_DIR build)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -std=c++11")

add_subdirectory (third_party/logger)
add_subdirectory (third_party/tarantool-c)
add_subdirectory (third_party/sqlparser)

include_directories(third_party/logger third_party/tarantool-c third_party/sqlparser/build/ ./)

set(sqlconn_src
	CreateMaker.cpp
	DropMaker.cpp
	InsertMaker.cpp
	SQLMaker.cpp
	SelectMaker.cpp
	datastructure.cpp
	mvalue.cpp
	tarantool_objs.cpp
	tarantool_schema_.cpp
	tp_wrap.cpp
	utils.cpp
)

add_library(sqlconn STATIC ${sqlconn_src})
add_dependencies(sqlconn logger tpc sqlparser)
target_link_libraries(sqlconn logger tpc sqlparser)

add_subdirectory (test)
