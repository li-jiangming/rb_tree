cmake_minimum_required(VERSION 3.10)

# Set some basic project attributes
project (main
	VERSION 0.1
	DESCRIPTION "A Hello World Project")

set(CMAKE_C_STANDARD 11)

add_subdirectory(ab_utils)

include_directories(.)
link_directories(ab_utils)

# This project will output an executable file
add_executable(${PROJECT_NAME} main.c rb_tree.c)

# Create a simple configuration header
# configure_file(config.h.in config.h)

# Include the configuration header in the build
# target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(${PROJECT_NAME} ab_utils)
