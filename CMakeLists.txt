

cmake_minimum_required(VERSION 4.1.0)

project(SPG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# find_package(Folly CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)


# The path to the C++ file should be relative to where this CMakeLists.txt file is
add_executable(
 SPG test/test.cpp
)

add_subdirectory(build/folly/)

target_include_directories(SPG
    PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries( SPG
    PUBLIC folly
)
