include_directories(/usr/include/lua5.1/ ${SQLITE_DIR}/tsrc/ ${SQLITE_DIR} third_party/)
link_directories(third_party/logger)
set(srcs driver.cpp ${SQLITE_DIR}/sqlite3.c)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
add_subdirectory(third_party/logger)

add_library(driver SHARED ${srcs})
target_link_libraries(driver dl pthread lua5.1 logger)
set_target_properties(driver PROPERTIES PREFIX "" OUTPUT_NAME "driver")

install(TARGETS driver LIBRARY DESTINATION ${TARANTOOL_LIBDIR}/sqlite/)
install(FILES third_party/logger/liblogger.a DESTINATION ${TARANTOOL_LIBDIR}/sqlite/)
install(FILES init.lua DESTINATION ${TARANTOOL_LUADIR}/sqlite/)

