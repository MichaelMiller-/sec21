cmake_minimum_required(VERSION 3.1)
project(sec21 CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Release)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

## global settings
#set(GLOBAL_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
#set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")

## compiler flags
# if (not MSVC)
#     set(CMAKE_CXX_STANDARD 14)
# endif()

include_directories(include)

# enable dashboard scripting

## to disable testing set BUILD_TESTING to false
include(CTest)
enable_testing()
add_subdirectory(tests)

