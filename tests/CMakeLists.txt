add_subdirectory(queue)
add_subdirectory(interpolate)
add_subdirectory(poll)

enable_testing()

add_test(NAME queue_test COMMAND $<TARGET_FILE:queue_test>)
add_test(NAME interpolate_test COMMAND $<TARGET_FILE:interpolate_test>)
add_test(NAME poll_test COMMAND $<TARGET_FILE:poll_test>)