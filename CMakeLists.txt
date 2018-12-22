cmake_minimum_required(VERSION 3.1)
project(sec21 CXX)

## set(CMAKE_BUILD_TYPE "Release")

include_directories("include")

if (CMAKE_COMPILER_IS_GNUCC)
   option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)

   if (ENABLE_COVERAGE)
      add_compile_options(--coverage -O0)
   endif()
endif()

## target_compile_features(binaryname PRIVATE cxx_lambda_init_captures)

## local Windows
## cmake .. "-DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake"

## enable benchmarks 
## add_subdirectory(benchmarks)
# enable dashboard scripting

## to disable testing set BUILD_TESTING to false
include(CTest)
enable_testing()
add_subdirectory(tests)
