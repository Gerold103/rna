# cmake_minimum_required(VERSION 2.8)
# project(tpc)

include_directories (msgpuck)

set(tpc_src
	session.c
	tp.c
	third_party/base64.c
	third_party/sha1.c
)

add_library(tpc STATIC ${tpc_src})
