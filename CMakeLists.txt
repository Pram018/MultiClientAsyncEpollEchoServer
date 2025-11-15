cmake_minimum_required(VERSION 3.10)
project(http-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Include directories
include_directories(include)

# Executable
add_executable(server ${SOURCES})

