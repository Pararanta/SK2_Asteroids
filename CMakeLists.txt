cmake_minimum_required(VERSION 3.25.1)

project(Asteroids)

add_executable(${PROJECT_NAME} src/main.c)

add_subdirectory(vendor/glfw)

target_include_directories(${PROJECT_NAME}
    PUBLIC vendor/glfw/include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE vendor/glfw/src
)

target_link_libraries(${PROJECT_NAME}
    glfw
)