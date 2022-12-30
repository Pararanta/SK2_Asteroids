add_library(gui INTERFACE)
target_link_libraries(gui
    INTERFACE
        ext
)
target_include_directories(gui INTERFACE include)
target_sources(gui INTERFACE glfw.c gui.c)