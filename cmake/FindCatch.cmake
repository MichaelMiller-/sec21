cmake_minimum_required(VERSION 2.8.2)

project(catch-download NONE)

## TODO: check FIND_VERSION_XXX

## possible to download the single header
## https://raw.githubusercontent.com/CatchOrg/Catch2/master/single_include/catch.hpp

include(ExternalProject)
ExternalProject_Add(catch
  GIT_REPOSITORY    "https://github.com/catchorg/Catch2.git"
  GIT_TAG           "v${Catch_FIND_VERSION_MAJOR}.${Catch_FIND_VERSION_MINOR}.${Catch_FIND_VERSION_MINOR}"
  SOURCE_DIR        "${CMAKE_BINARY_DIR}/catch-src"
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

set (Catch_ROOT "${CMAKE_BINARY_DIR}/catch-src")
set (Catch_INCLUDE_DIR "${Catch_ROOT}/single_include")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Catch DEFAULT_MSG Catch_INCLUDE_DIR)

mark_as_advanced(Catch_INCLUDE_DIR)

set (CATCH_INCLUDE_DIRS "${Catch_INCLUDE_DIR}")
