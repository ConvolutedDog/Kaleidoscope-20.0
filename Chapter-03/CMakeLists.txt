cmake_minimum_required(VERSION 3.10)
project(Kaleidoscope-Chpter-03)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use command `cmake .. -DUSE_CLANG=ON` to enable Clang compiler.
option(USE_CLANG "Use Clang compiler" OFF)

if(USE_CLANG)
  # Try to find Clang compilers.
  find_program(CLANG_C_COMPILER NAMES clang)
  find_program(CLANG_CXX_COMPILER NAMES clang++)
  if(CLANG_C_COMPILER AND CLANG_CXX_COMPILER)
    set(CMAKE_C_COMPILER ${CLANG_C_COMPILER})
    set(CMAKE_CXX_COMPILER ${CLANG_CXX_COMPILER})
  else()
    message(FATAL_ERROR "Clang compilers not found")
  endif()
else()
  set(CMAKE_C_COMPILER gcc)
  set(CMAKE_CXX_COMPILER g++)
endif()

message(STATUS "Using CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "Using CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

# Use `cmake .. -DBUILD_DEBUG` to debug.
option(BUILD_DEBUG "Build in debug mode" OFF)

if(BUILD_DEBUG)
  set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Included LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

file(GLOB LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.cpp")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/lib/Kaleidoscope.cpp")
add_library(KaleidoscopeLib STATIC ${LIB_SOURCES})

target_include_directories(KaleidoscopeLib PUBLIC include ${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/lib/Kaleidoscope.cpp")
add_executable(Kaleidoscope-Chpter-03 ${SOURCES})
target_include_directories(Kaleidoscope-Chpter-03 PRIVATE include ${LLVM_INCLUDE_DIRS})
target_link_libraries(Kaleidoscope-Chpter-03 KaleidoscopeLib)

# Find the libraries that correspond to the LLVM components that we wish to use.
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries.
target_link_libraries(Kaleidoscope-Chpter-03 ${llvm_libs})
