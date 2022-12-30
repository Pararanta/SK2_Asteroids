add_executable(interpolation_test interpolate_test.c)
target_compile_definitions(interpolation_test PUBLIC STEPS=1000)
target_compile_definitions(interpolation_test PUBLIC TIME_STEP=0.05f)
target_compile_definitions(interpolation_test PUBLIC EPSILON=0.001f)

target_link_libraries(interpolation_test
    PUBLIC
        utils
)