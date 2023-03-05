cmake_minimum_required(VERSION 3.22.1)

# Default project config
project(cSnakeGame VERSION 0.8) # Project name and version
add_executable(cSnakeGame src/main.c) # Defines the executable name
target_link_libraries(cSnakeGame curses) # Include some libs if needed
install(TARGETS cSnakeGame DESTINATION bin) # Tells where to store the binaries
