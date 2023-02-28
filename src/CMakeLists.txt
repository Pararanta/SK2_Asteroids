add_subdirectory(gui)
add_subdirectory(shared)

add_executable(${PROJECT_NAME}_server server.c)
add_executable(${PROJECT_NAME}_client client.c)

target_link_libraries(${PROJECT_NAME}_server
    gui
    shared
    tinycthread
)
target_include_directories(${PROJECT_NAME}_server PUBLIC include)

target_link_libraries(${PROJECT_NAME}_client
    gui
    shared
    tinycthread
)
target_include_directories(${PROJECT_NAME}_client PUBLIC include)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(${PROJECT_NAME}_server
        ws2_32
    )
    target_link_libraries(${PROJECT_NAME}_client
        ws2_32
    )
endif()
