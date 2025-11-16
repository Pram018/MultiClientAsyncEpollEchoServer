cmake_minimum_required(VERSION 4.1.2)
project(http-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

# Threadpool server (modular version)
add_executable(threadpool_server
    src/main.cpp
    src/server.cpp
    src/ThreadPool.cpp
)
