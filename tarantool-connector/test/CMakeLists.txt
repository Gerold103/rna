set (test_src main.cpp test_utils.cpp)

add_executable(main ${test_src})
target_link_libraries(main sqlconn)
