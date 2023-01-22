find_package( OpenGL REQUIRED )
find_package( Python3 REQUIRED COMPONENTS Interpreter)

execute_process(
        COMMAND pip show glad2
        RESULT_VARIABLE EXIT_CODE
        OUTPUT_QUIET
)

if (NOT ${EXIT_CODE} EQUAL 0)
    execute_process(COMMAND pip install .
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/ext/glad)
endif()


set( GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/ext/glad" )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

add_subdirectory( glfw )
add_subdirectory( "${GLAD_SOURCES_DIR}/cmake" glad_cmake )

glad_add_library(glad_gl_core STATIC API gl:core=3.3)

add_subdirectory( tinycthread )

add_library(ext INTERFACE)

target_link_libraries(ext
    INTERFACE
        glfw
        glad_gl_core
        tinycthread
)

target_include_directories(ext
    INTERFACE
    glfw/include
    ${OPENGL_INCLUDE_DIRS}
)

target_link_directories(ext
    INTERFACE
    glfw/src
)