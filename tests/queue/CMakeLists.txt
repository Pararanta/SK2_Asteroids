add_executable(queue_test queue_test.c)
target_compile_definitions(queue_test PUBLIC MAX_QUEUE_SIZE_BYTES=9)
target_link_libraries(queue_test
    PUBLIC
        utils
)