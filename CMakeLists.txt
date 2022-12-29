cmake_minimum_required( VERSION 3.25.1 )

project( Asteroids
    LANGUAGES C 
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(GUI_FILES
    ${SRC_DIR}/gui/glfw.c   ${SRC_DIR}/gui/glfw.h
    ${SRC_DIR}/gui/gui.c    ${SRC_DIR}/gui/gui.h
)
add_executable(${PROJECT_NAME} src/main.c ${GUI_FILES})

find_package( OpenGL REQUIRED )

execute_process(COMMAND pip install .
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/ext/glad)

set( GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/ext/glad" )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

add_subdirectory( ext/glfw )
add_subdirectory( "${GLAD_SOURCES_DIR}/cmake" glad_cmake )

glad_add_library(glad_gl_core STATIC API gl:core=3.3)

target_include_directories(${PROJECT_NAME}
    PUBLIC ext/glfw/include
    ${OPENGL_INCLUDE_DIRS}
)

target_link_directories(${PROJECT_NAME}
    PRIVATE ext/glfw/src
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        glad_gl_core
        glfw
)