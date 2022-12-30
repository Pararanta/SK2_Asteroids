add_subdirectory(queue)

enable_testing()

add_test(NAME queue_test COMMAND $<TARGET_FILE:queue_test>)