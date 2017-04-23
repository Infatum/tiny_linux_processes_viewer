cmake_minimum_required(VERSION 3.7)
project(linux_process_viewer)

find_package(Boost 1.54.0 COMPONENTS regex REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp src/DirectoryParser.cpp src/DirectoryParser.h src/PID_Table.h
        src/Process_manager.h src/Process_manager.cpp src/Directories.h)
add_executable(linux_process_viewer ${SOURCE_FILES})
target_link_libraries(linux_process_viewer ${Boost_LIBRARIES})