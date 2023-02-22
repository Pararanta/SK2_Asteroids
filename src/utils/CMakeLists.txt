add_library(utils INTERFACE)
target_include_directories(utils INTERFACE include)
target_sources(utils INTERFACE socket.c sleep.c)