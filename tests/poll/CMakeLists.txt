add_executable(poll_test poll_test.c)

target_link_libraries(poll_test
    PUBLIC
        utils
)