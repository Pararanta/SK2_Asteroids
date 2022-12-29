cmake_minimum_required(VERSION 3.25.1)

project(Asteroids C)

find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

add_executable(${PROJECT_NAME} src/main.c)

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/vendor/glad")

add_subdirectory( vendor/glfw )
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)

glad_add_library(glad_gl_core STATIC API gl:core=3.3)

target_include_directories(${PROJECT_NAME}
    PUBLIC vendor/glfw/include
)

target_link_directories(${PROJECT_NAME}
    PRIVATE vendor/glfw/src
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        glad_gl_core
        glfw
)