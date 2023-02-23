add_library(shared INTERFACE)
target_include_directories(shared INTERFACE include)
target_sources(shared INTERFACE socket.c sleep.c messages.c player.c extensions.c)