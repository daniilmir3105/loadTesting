cmake_minimum_required(VERSION 3.25)
project(loadTesting)

set(CMAKE_CXX_STANDARD 17)

add_executable(loadTesting simple_server.cpp http_server.cpp test.cpp)

