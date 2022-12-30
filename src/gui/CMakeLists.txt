add_library(gui INTERFACE glfw.c gui.c)
target_link_libraries(gui
    INTERFACE
        ext
)
target_include_directories(gui INTERFACE include)