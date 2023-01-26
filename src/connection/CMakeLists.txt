add_library(connection INTERFACE)
target_include_directories(connection INTERFACE include)
target_sources(connection INTERFACE player.c)