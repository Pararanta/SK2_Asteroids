add_subdirectory(queue)
add_subdirectory(interpolate)

enable_testing()

add_test(NAME queue_test COMMAND $<TARGET_FILE:queue_test>)
add_test(NAME interpolate_test COMMAND $<TARGET_FILE:interpolate_test>)