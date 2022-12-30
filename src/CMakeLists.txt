add_subdirectory(gui)
add_subdirectory(utils)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        gui
        utils
)