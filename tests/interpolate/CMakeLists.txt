add_executable(interpolate_test interpolate_test.c)
target_compile_definitions(interpolate_test PUBLIC STEPS=10000)
target_compile_definitions(interpolate_test PUBLIC TIME_STEP=0.005f)
target_compile_definitions(interpolate_test PUBLIC EPSILON=0.001f)

target_link_libraries(interpolate_test
    PUBLIC
        utils
)