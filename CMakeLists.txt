cmake_minimum_required(VERSION 3.7)
project(linux_process_viewer)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(linux_process_viewer ${SOURCE_FILES})