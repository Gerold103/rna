# project(logger)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(logger_src
	logger.cpp
	timeformatter.cpp
)
add_library(logger STATIC ${logger_src})
