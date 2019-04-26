cmake_minimum_required(VERSION 3.1)
project(sec21 CXX)

## set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

include_directories("include")

if (CMAKE_COMPILER_IS_GNUCXX)
   option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" TRUE)

   if (ENABLE_COVERAGE)
      message("-- Coverage is enabled")
      add_compile_options(--coverage -O0)
   endif()
endif()

## display with http://webgraphviz.com/
# add_custom_target(
#    graphviz ALL
#    "${CMAKE_COMMAND}" "--graphviz=graphviz.sec21" .
#    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
#    )

## local Windows
# cmake .. "-DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake"

## enable benchmarks 
# add_subdirectory(benchmarks)
# add_subdirectory(portal)

## to disable testing set BUILD_TESTING to false
include(CTest)
enable_testing()
add_subdirectory(tests)

## TODO: enable dashboard scripting
