cmake_minimum_required(VERSION 3.1)
project(sec21 CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Release)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

## TODO: CPack and CDash 

## https://stackoverflow.com/questions/20746936/cmake-of-what-use-is-find-package-if-you-need-to-specify-cmake-module-path-an
## find_package(Foo REQUIRED) # FOO_INCLUDE_DIR, FOO_LIBRARIES
## sfind_package(Boost 1.63 REQUIRED) # BOO_INCLUDE_DIR, BOO_LIBRARIES

## global settings
#set(GLOBAL_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
#set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")

## compiler flags
# if (not MSVC)
#     set(CMAKE_CXX_STANDARD 14)
# endif()

## include cmake module https://cmake.org/cmake/help/v3.0/module/ExternalProject.html
# include(ExternalProject)

## TODO: ExternalProject_Add vs.find_package()
#ExternalProject_Add(
#  wt
#  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/wt
#  CMAKE_ARGS
#    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
#    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
#
#ExternalProject_Add_Step(
#  wt CopyToBin
#  COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/wt/lib ${GLOBAL_OUTPUT_PATH}
#  DEPENDEES install)
  

# ExternalProject_Get_Property(project_a install_dir)
# include_directories(${install_dir}/include
# set(WT_INCLUDE_DIRS ${GLOBAL_OUTPUT_PATH}/wt-prefix/include)
# set(WT_LIBRARIES ${CMAKE_SHARED_LIBRARY_PREFIX}wt${CMAKE_SHARED_LIBRARY_SUFFIX})
  
##
## stack upgrade to update haskell https://docs.haskellstack.org/en/stable/install_and_upgrade/#ubuntu
## BUILD_COMMAND and INSTALL_COMMAND not specified -> cmake looks for CMakeLists.txt in the external
# ExternalProject_Add(
#   bond
#   #--Download step--------------
#   ## TODO: possible from git, svn, hg, http, file, ...
#   #--Configure step-------------
#   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/bond
#   CMAKE_ARGS 
#     -DBOND_ENABLE_GRPC=FALSE
#     -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})


include_directories(include)

# function(echo_all_cmake_variable_values)
#   message(STATUS “”)
#   get_cmake_property(vs VARIABLES)
#   foreach(v ${vs})
#     message(STATUS “${v}=’${${v}}'”)
#   endforeach(v)
#   message(STATUS “”)
# endfunction()
# echo_all_cmake_variable_values()

# enable dashboard scripting

## to disable testing set BUILD_TESTING to false
include(CTest)
enable_testing()
add_subdirectory(tests)

