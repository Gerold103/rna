set (test_src ./sqlexprs/main.cpp ./sqlexprs/test_utils.cpp)

add_executable(main ${test_src})

set_target_properties(main
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "sqlexprs"
)

target_link_libraries(main sqlconn)
