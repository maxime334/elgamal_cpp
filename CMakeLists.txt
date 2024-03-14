cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the source and include directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Gather the source files
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)

# Create an executable target
add_executable(myapp ${SRC_FILES})

# Include directories
target_include_directories(myapp PRIVATE ${INCLUDE_DIR})
