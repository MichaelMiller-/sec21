cmake_minimum_required(VERSION 3.1)
project(sec21 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")

include_directories("include")

## Windows
## cmake .. "-DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake"

## enable benchmarks 
## add_subdirectory(benchmarks)

# enable dashboard scripting

## to disable testing set BUILD_TESTING to false
include(CTest)
enable_testing()
add_subdirectory(tests)
