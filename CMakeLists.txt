cmake_minimum_required(VERSION 3.1)

## 
## https://github.com/sutambe/cpptruths/
## http://cpptruths.blogspot.com/2019/03/bootstrapping-vcpkg-based-cmake-project_31.html
##
## set(MY_PROJECT_DEPENDENCIES boost) 
# This section of cmake is using AutoVcpkg to download, install, and configure vcpkg.
## list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
## include(AutoVcpkg)
## vcpkg_install(${MY_PROJECT_DEPENDENCIES})
message(STATUS "CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")

## TODO: OpenMP Support is improved if cmake version is greater then 3.9
project(sec21 CXX)

## set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

include_directories("include")

find_package(OpenMP)
if (OPENMP_FOUND)
   message("-- OpenMP Support is enabled")
   ## TODO
   # set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
   # set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
   # set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
   option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)

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

option(ENABLE_BENCHMARKS "Enable benchmarks" FALSE)
option(ENABLE_DATABASE_SERIALIZER "Enable database serlializer" FALSE)
option(ENABLE_VIEWER "Enable system viewer" FALSE)
option(ENABLE_WEBINTERFACE "Enable web interface" FALSE)

if (ENABLE_BENCHMARKS)
   message("-- Benchmarks is enabled")
   configure_file(${CMAKE_SOURCE_DIR}/benchmarks.sqlite3 ${CMAKE_BINARY_DIR}/benchmarks.sqlite3 COPYONLY)
   add_subdirectory(benchmarks)
endif()

if (ENABLE_WEBINTERFACE)
   message("-- Wt Web interface is enabled")
   add_subdirectory(portal)
endif()   

if (ENABLE_DATABASE_SERIALIZER)
   message("-- Wt DBO database serializer is enabled")
   add_subdirectory(database-serializer)
endif()

if (ENABLE_VIEWER)
   message("-- OpenGL SDL system viewer is enabled")
   add_subdirectory(viewer)
endif()

## to disable testing set BUILD_TESTING to false
include(CTest)
enable_testing()
add_subdirectory(tests)
## -- or
## include(Catch)
## catch_discover_tests(foo)
## -- or
## include(ParseAndAddCatchTests)
## ParseAndAddCatchTests(foo)
## -- or

## TODO: enable dashboard scripting
