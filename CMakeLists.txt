cmake_minimum_required(VERSION 2.8)

project(sqlparser)

set(BUILD_DIR build)

include_directories(postgres postgres/src postgres/src/backend postgres/src/common postgres/src/include postgres/src/interfaces postgres/src/port postgres/src/template postgres/src/timezone postgres/src/tools)

file(GLOB_RECURSE srcs ./ postgres/src/backend/*.c postgres/src/common/*.c postgres/src/include/*.c postgres/src/interfaces/*.c postgres/src/port/*.c postgres/src/template/*.c postgres/src/timezone/*.c postgres/src/tools/*.c)

add_executable(main ${srcs})