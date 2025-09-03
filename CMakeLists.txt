cmake_minimum_required(VERSION 3.22.1)


project(SPG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# find_package(Folly CONFIG REQUIRED) # Enable for access without relying on the folly the submodule
find_package(gflags CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

add_executable(SPG 
    test/Test.cpp
    ${SRC_FILES}
)
add_executable(astar 
    test/astar.cpp
    ${SRC_FILES}
)
add_executable(dijkstra 
    test/dijkstra.cpp
    ${SRC_FILES}
)
add_executable(multithreaded_astar 
    test/multithreaded_astar.cpp
    ${SRC_FILES}
)

set(BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(BUILD_BENCHMARKS OFF CACHE INTERNAL "" FORCE)
add_subdirectory(external/folly/)

target_include_directories(SPG
    PUBLIC ${CMAKE_SOURCE_DIR}/src
    PUBLIC ${CMAKE_SOURCE_DIR}/src/utils
    PUBLIC ${CMAKE_SOURCE_DIR}/src/graph
    PUBLIC ${CMAKE_SOURCE_DIR}/src/data
)
target_include_directories(astar
    PUBLIC ${CMAKE_SOURCE_DIR}/src
    PUBLIC ${CMAKE_SOURCE_DIR}/src/utils
    PUBLIC ${CMAKE_SOURCE_DIR}/src/graph
    PUBLIC ${CMAKE_SOURCE_DIR}/src/data
)
target_include_directories(multithreaded_astar
    PUBLIC ${CMAKE_SOURCE_DIR}/src
    PUBLIC ${CMAKE_SOURCE_DIR}/src/utils
    PUBLIC ${CMAKE_SOURCE_DIR}/src/graph
    PUBLIC ${CMAKE_SOURCE_DIR}/src/data
)
target_include_directories(dijkstra
    PUBLIC ${CMAKE_SOURCE_DIR}/src
    PUBLIC ${CMAKE_SOURCE_DIR}/src/utils
    PUBLIC ${CMAKE_SOURCE_DIR}/src/graph
    PUBLIC ${CMAKE_SOURCE_DIR}/src/data
)

target_link_libraries( SPG
    PUBLIC folly                    # Here, for access without relying on the submodule, instead of <folly> write <Folly::folly>
)
target_link_libraries( astar
    PUBLIC folly                    # Here, for access without relying on the submodule, instead of <folly> write <Folly::folly>
)
target_link_libraries( multithreaded_astar
    PUBLIC folly                    # Here, for access without relying on the submodule, instead of <folly> write <Folly::folly>
)
target_link_libraries( dijkstra
    PUBLIC folly                    # Here, for access without relying on the submodule, instead of <folly> write <Folly::folly>
)