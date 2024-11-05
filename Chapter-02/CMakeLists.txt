cmake_minimum_required(VERSION 3.10)
project(Kaleidoscope-Chpter-02)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCES "lib/*.cpp")
add_executable(Kaleidoscope-Chpter-02 ${SOURCES})
