cmake_minimum_required(VERSION 3.10)
project(http-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(include)

# Baseline server (your original single-threaded server.cpp)
add_executable(baseline_server src/server.cpp)

# Threadpool server (modular version with Server + ThreadPool + main)
add_executable(threadpool_server
    src/main.cpp
    src/Server.cpp
    src/ThreadPool.cpp
)
