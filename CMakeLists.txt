# # cmake_minimum_required(VERSION 3.31.5)

# # project(SPG)

# # include_directories( src )

# # add_executable(${PROJECT_NAME} test/test.cpp)

# # set_property(TARGET SPG PROPERTY CXX_STANDARD 23)


# cmake_minimum_required(VERSION 3.7)
# find_package(Boost REQUIRED)
# find_package(folly REQUIRED)
# find_package(Threads REQUIRED)
# find_package(gflags REQUIRED)

# set(CMAKE_CXX_STANDARD 14)
# #include_directories(${Boost_INCLUDE_DIRS})
# #include_directories(${folly_INCLUDE_DIRS})

# set_and_check(FOLLY_INCLUDE_DIR /folly)
# set_and_check(FOLLY_CMAKE_DIR /lib/cmake/folly)
# if (NOT TARGET Folly::folly)
#   include("${FOLLY_CMAKE_DIR}/folly-targets.cmake")
# endif()


# set(FOLLY_LIBRARIES Folly::folly)

# if (NOT folly_FIND_QUIETLY)
#   message(STATUS "Found folly: ${PACKAGE_PREFIX_DIR}")
# endif()

# add_executable(HelloWorld test/test.cpp)
# target_link_libraries(HelloWorld ${Boost_LIBRARIES} ${FOLLY_LIBRARIES})


# # add_subdirectory(src)

# # add_subdirectory(external/glfw)

# # target_include_directories(${PROJECT_NAME} 
# #     PUBLIC src 
# #     # PUBLIC external/glfw/include
# # )

# # target_link_directories(${PROJECT_NAME} 
# #     PRIVATE src
# #     # PRIVATE external/glfw/src
# # )

# # target_link_libraries(${PROJECT_NAME} src)








# cmake_minimum_required(VERSION 3.31.5)
# project(SPG LANGUAGES CXX)
# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# find_package(folly REQUIRED)
# find_package(Boost REQUIRED)
# # find_package(folly REQUIRED)
# add_executable(SPG test/test.cpp)
# target_link_libraries(SPG PRIVATE Folly::folly)


cmake_minimum_required(VERSION 3.16)
project(SPG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Make sure CMake searches Homebrew's install prefix
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

# Find folly (this will also find Boost, gflags, etc. via folly's config)
find_package(folly REQUIRED)
find_package(folly CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)

add_executable(SPG test/test.cpp)

# Link against folly
target_link_libraries(SPG PRIVATE Folly::folly)

target_include_directories(SPG PRIVATE
    src/Graph.hpp
)
