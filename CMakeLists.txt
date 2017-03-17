cmake_minimum_required(VERSION 2.8)
project(sec21 CXX)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -Wno-reorder -Wno-braced-scalar-init -pedantic -O3" )
endif()

## todo: available with cmake 3.1
## set (CMAKE_CXX_STANDARD 11)

include_directories(include)
include_directories(deps/Catch/include)

## if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
if (MSVC)
	include_directories(deps/range-v3-vs2015/include)	
else()
	include_directories(deps/range-v3/include)	
endif()

enable_testing()

add_subdirectory(test)
