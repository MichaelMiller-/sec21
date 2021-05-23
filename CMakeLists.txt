## TODO: OpenMP Support is improved if cmake version is greater then 3.9
cmake_minimum_required(VERSION 3.9)

project(sec21 CXX)
set(SEC21_CMAKE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(${SEC21_CMAKE_PATH}/StandardProjectSettings.cmake)

set(${PROJECT_NAME}_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(${PROJECT_NAME}_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(${PROJECT_NAME}_VERSION_PATCH 0 CACHE STRING "patch version" FORCE)
set(
    ${PROJECT_NAME}_VERSION 
    ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH} 
    CACHE STRING "version" 
    FORCE)

## target_compile_features(myTarget PUBLIC cxx_std_11)

# if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.11.0")
#    file(GLOB_RECURSE header_files CONFIGURE_DEPENDS ${header_path}/*.h)
# else()
#    file(GLOB header_files ${header_path}/**.h)
# endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

## header-only library
add_library(${PROJECT_NAME} INTERFACE)

# TODO
#if(NOT TARGET sec21::sec21)
  #include(${SEC21_CMAKE_PATH}/sec21Targets.cmake)
#endif()

# configure_file(${PROJECT_SOURCE_DIR}/cmake/FindDebViennaCL.cmake FindDebViennaCL.cmake COPYONLY)
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_BINARY_DIR})
# # ## todo: use config
# find_package(DebViennaCL REQUIRED)

find_package(Boost 1.70 REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} INTERFACE Boost::boost)
target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json nlohmann_json::nlohmann_json)
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE include)

# TARGETS_EXPORT_NAME
# target_include_directories(${PROJECT_NAME} PUBLIC
#                        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> # for headers when building
#                         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # for config_impl.hpp when building
#                        $<INSTALL_INTERFACE:${include_dest}> # for client in install mode
#                        $<INSTALL_INTERFACE:${lib_dest}> # for config_impl.hpp in install mode)
# )

include(${SEC21_CMAKE_PATH}/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME})

include(${SEC21_CMAKE_PATH}/Sanitizers.cmake)
enable_sanitizers(${PROJECT_NAME})

include(${SEC21_CMAKE_PATH}/Doxygen.cmake)
enable_doxygen()

include(${SEC21_CMAKE_PATH}/StaticAnalyzers.cmake)

configure_file("${PROJECT_SOURCE_DIR}/include/sec21/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h")

option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
   target_precompile_headers(sec21 INTERFACE <vector> <string> <map> <utility> <algorithm> <numeric>)
endif()

option(ENABLE_TESTS "Enable tests" OFF)
option(ENABLE_BENCHMARKS "Enable benchmarks" ON)
option(ENABLE_DATABASE_SERIALIZER "Enable database serlializer" OFF)
option(ENABLE_VIEWER "Enable system viewer" OFF)
option(ENABLE_WEB_INTERFACE "Enable web interface" OFF)
option(ENABLE_PLOTTER "Enable plotter" OFF)
option(ENABLE_COLLECT "Enable collect" OFF)
option(ENABLE_SCRIPT_INTERFACE "Enable script interface" OFF)
option(ENABLE_EXPERIMENTAL "Enable experimental mode" OFF)

if (ENABLE_BENCHMARKS)
   message(STATUS "Benchmarks enabled")
#   add_subdirectory(benchmarks)
endif()

if (ENABLE_WEB_INTERFACE)
   message(STATUS "Wt Web interface is enabled")
   add_subdirectory(portal)
endif()   

if (ENABLE_DATABASE_SERIALIZER)
   message(STATUS "Database serializer is enabled")
   add_subdirectory(database-serializer)
endif()

if (ENABLE_VIEWER)
   message(STATUS "OpenGL system viewer is enabled")
   add_subdirectory(viewer)
endif()

if (ENABLE_PLOTTER)
   message(STATUS "Plotter is enabled")
   add_subdirectory(plotter)
endif()

if (ENABLE_COLLECT)
   message(STATUS "Collect is enabled")
   add_subdirectory(collect)
endif()

if (ENABLE_SCRIPT_INTERFACE)
   message(STATUS "Lua script interface is enabled")
   add_subdirectory(scripting)
endif()  

add_subdirectory(orm_compiler)

## TODO
# if (ENABLE_FUZZING)
#    message(STATUS "Fuzzing is enabled")
#    add_subdirectory(fuzz_tests)
# endif()  

#if (ENABLE_TESTS)
#   include(CTest)
#   enable_testing()
#   add_subdirectory(tests)
#endif()

## copy optional data 
if(EXISTS "${PROJECT_SOURCE_DIR}/data/cp_gen.sh")
   configure_file("${PROJECT_SOURCE_DIR}/data/cp_gen.sh" "${PROJECT_BINARY_DIR}/cp_gen.sh" COPYONLY)
endif()


# add_subdirectory(worker)
# add_subdirectory(kraken)
# add_subdirectory(allocator_awareness)
# add_subdirectory(translate)
# add_subdirectory(webapp)
## TODO: was working: add_subdirectory(websocket_client)
add_subdirectory(mesh_processing)
add_subdirectory(library1)
add_subdirectory(library1_test)
add_subdirectory(runtime_plugin_system) ## servlet like / runtimesystem
add_subdirectory(compiletime1)
add_subdirectory(compiletime2)
add_subdirectory(example_json)


##
##  
## Deep CMake for Library Authors - Craig Scott - CppCon 2019



#ary(Foo ...)
# Make override a feature requirement only if available
#target_compile_features(Foo PUBLIC
# $<$<COMPILE_FEATURES:cxx_override>:cxx_override>
#)
# Define the foo_OVERRIDE symbol so it provides the
# override keyword if available or empty otherwise
#target_compile_definitions(Foo PUBLIC
# $<$<COMPILE_FEATURES:cxx_override>:-Dfoo_OVERRIDE=override>
# $<$<NOT:$<COMPILE_FEATURES:cxx_override>>:-Dfoo_OVERRIDE>
#)

#include(WriteCompilerDetectionHeader)
#write_compiler_detection_header(
# FILE foo_compiler_detection.h
# PREFIX foo
# COMPILERS GNU Clang MSVC Intel
# FEATURES cxx_override
# cxx_final
# cxx_nullptr
# cxx_rvalue_references
#)

#include "foo_compiler_detection.h"
#class MyClass foo_FINAL : public Base
#{
#public:
#if foo_COMPILER_CXX_RVALUE_REFERENCES
# MyClass(MyClass&& c);
#endif
# void func1() foo_OVERRIDE;
# void func2(int* p = foo_NULLPTR);
#};


# Aliases to the above with special names
#add_library(BagOfBeans::MyRealThings ALIAS MyRealThings)
#add_library(BagOfBeans::OtherThings ALIAS OtherThings)
# Pull in imported targets from an installed package.
# See details in Chapter 23: Finding Things
#find_package(BagOfBeans REQUIRED)
# Define an executable that links to the imported
# library from the installed package
#add_executable(EatLunch main.cpp ...)
#target_link_libraries(EatLunch PRIVATE
# BagOfBeans::MyRealThings
#)


# Now the library has been split in two, so define
# an interface library with the old name to effectively
# forward on the link dependency to the new libraries
#add_library(ComputeAlgoA SHARED ...)
#add_library(ComputeAlgoB SHARED ...)
#add_library(DeepCompute INTERFACE)
#target_link_libraries(DeepCompute INTERFACE
# ComputeAlgoA
# ComputeAlgoB
#)



#execute_process(COMMAND runSomeScript
# RESULT_VARIABLE result)
#if(result)
# message(FATAL_ERROR "runSomeScript failed: ${result}")
#endif()


#set(BAR "Some \"quoted\" value")
#configure_file(quoting.txt.in quoting.txt)
#configure_file(quoting.txt.in quoting_escaped.txt ESCAPE_QUOTES)


# Generate unique files for all but Release
#file(GENERATE
# OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/outFile-$<CONFIG>.txt
# INPUT ${CMAKE_CURRENT_SOURCE_DIR}/input.txt.in
# CONDITION $<NOT:$<CONFIG:Release>>
#)

#cmake_minimum_required(VERSION 3.0)
#include(projectVersionDetails.cmake)
#project(FooBar VERSION ${FooBar_VER})
#find_package(Git REQUIRED)
#execute_process(
# COMMAND ${GIT_EXECUTABLE} rev-list -1 HEAD projectVersionDetails.cmake
# RESULT_VARIABLE result
# OUTPUT_VARIABLE lastChangeHash
# OUTPUT_STRIP_TRAILING_WHITESPACE
#)
#if(result)
# message(FATAL_ERROR "Failed to get hash of last change: ${result}")
#endif()
#execute_process(
# COMMAND ${GIT_EXECUTABLE} rev-list ${lastChangeHash}..HEAD
# RESULT_VARIABLE result
# OUTPUT_VARIABLE hashList
# OUTPUT_STRIP_TRAILING_WHITESPACE
#)

#if(result)
# message(FATAL_ERROR "Failed to get list of git hashes: ${result}")
#endif()
#string(REGEX REPLACE "[\n\r]+" ";" hashList "${hashList}")
#list(LENGTH hashList FooBar_COMMITS_SINCE_VERSION_CHANGE)
#configure_file(foobar_version.cpp.in foobar_version.cpp @ONLY)
# Targets, etc....
#include "foobar_version.h"
#std::string getFooBarVersion()
#{
# return "@FooBar_VERSION@";
#}
#unsigned getFooBarVersionMajor()
#{
# return @FooBar_VERSION_MAJOR@;
#}
#unsigned getFooBarVersionMinor()
#{
# return @FooBar_VERSION_MINOR@ +0;
#}
#unsigned getFooBarVersionPatch()
#{
# return @FooBar_VERSION_PATCH@ +0;
#}
#unsigned getFooBarVersionTweak()
#{
# return @FooBar_VERSION_TWEAK@ +0;
#}

##
#add_library(MyStuff SHARED source1.cpp ...)
#set_target_properties(MyStuff PROPERTIES
# VERSION 2.4.3
# SOVERSION 2
#)

##
#add_library(Networking net.cpp)
#set_target_properties(Networking PROPERTIES
# COMPATIBLE_INTERFACE_BOOL SSL_SUPPORT
# INTERFACE_SSL_SUPPORT YES
#)
#add_library(Util util.cpp)
#set_target_properties(Util PROPERTIES
# COMPATIBLE_INTERFACE_BOOL SSL_SUPPORT
# INTERFACE_SSL_SUPPORT YES
#)
#add_executable(MyApp myapp.cpp)
#target_link_libraries(MyApp PRIVATE Networking Util)
#target_compile_definitions(MyApp PRIVATE
# $<$<BOOL:$<TARGET_PROPERTY:SSL_SUPPORT>>:HAVE_SSL>
#)

#####
#add_library(Foo ...)
# Conditionally add -fcode-hoisting option using variables
#if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7)
# target_compile_options(Foo PRIVATE -fcode-hoisting)
#endif()
# Same thing using generator expressions instead
#set(isGNU $<CXX_COMPILER_ID:GNU>)
#set(newEnough $<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,7>)
#target_compile_options(Foo PRIVATE $<$<AND:${isGNU},${newEnough}>:-fcode-hoisting>)

