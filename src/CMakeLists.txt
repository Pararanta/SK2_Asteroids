add_subdirectory(gui)
add_subdirectory(shared)

add_executable(${PROJECT_NAME}_server server.c)
add_executable(${PROJECT_NAME}_client client.c)

target_link_libraries(${PROJECT_NAME}_server
    PUBLIC
        gui
        shared
        ws2_32
        tinycthread
)
target_include_directories(${PROJECT_NAME}_server PUBLIC include)

target_link_libraries(${PROJECT_NAME}_client
    PUBLIC
        gui
        shared
        ws2_32
        tinycthread
)
target_include_directories(${PROJECT_NAME}_client PUBLIC include)
