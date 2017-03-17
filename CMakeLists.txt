cmake_minimum_required(VERSION 3.1)
project(sec21 CXX)

include_directories(include)
include_directories(deps/Catch/include)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	include_directories(deps/range-v3-vs2015/include)	
else()
	include_directories(deps/range-v3/include)	
endif()

enable_testing()

add_subdirectory(test)
