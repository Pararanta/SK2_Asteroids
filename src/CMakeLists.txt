add_subdirectory(gui)
add_subdirectory(utils)

add_executable(${PROJECT_NAME}_server server.c)
add_executable(${PROJECT_NAME}_client client.c)

target_link_libraries(${PROJECT_NAME}_server
    PUBLIC
        gui
        utils
)

target_link_libraries(${PROJECT_NAME}_client
    PUBLIC
        gui
        utils
)