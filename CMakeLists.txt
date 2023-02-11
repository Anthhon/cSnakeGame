cmake_minimum_required(VERSION 3.22.1)

# Default project config
project(cSnakeGame VERSION 0.1) # Project name and version
add_executable(cSnake main.c) # Defines the executable name
target_link_libraries(cSnake curses) # Include some libs if needed
install(TARGETS cSnake DESTINATION bin) # Tells where to store the binaries
