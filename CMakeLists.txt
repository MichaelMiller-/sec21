## TODO: OpenMP Support is improved if cmake version is greater then 3.9
cmake_minimum_required(VERSION 3.1)

project(sec21 CXX)
include(cmake/StandardProjectSettings.cmake)

## TOOD: should be a vcpkg dependency
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/deps/viennacl/cmake")

## dependencies
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  include(cmake/Vcpkg.cmake)
  vcpkg_install(nlohmann-json boost-geometry boost-math boost-outcome boost-ublas catch2)
  include(${CMAKE_TOOLCHAIN_FILE})
endif()

## header-only library
add_library(${PROJECT_NAME} INTERFACE)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json nlohmann_json::nlohmann_json)
target_include_directories(sec21 SYSTEM INTERFACE include)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(sec21)

include(cmake/Sanitizers.cmake)
enable_sanitizers(sec21)

include(cmake/Doxygen.cmake)
enable_doxygen()

include(cmake/StaticAnalyzers.cmake)

option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
   target_precompile_headers(sec21 INTERFACE <vector> <string> <map> <utility> <algorithm>)
endif()

### 
## TODO: cleanup

find_package(OpenMP)
if (OPENMP_FOUND)
   set(ENABLE_OPENMP ON)
   ## message(STATUS "OpenMP Support is enabled")
   # TODO
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
## TODO: cleanup


option(ENABLE_TESTS "Enable tests" ON)
option(ENABLE_BENCHMARKS "Enable benchmarks" FALSE)
option(ENABLE_DATABASE_SERIALIZER "Enable database serlializer" FALSE)
option(ENABLE_VIEWER "Enable system viewer" FALSE)
option(ENABLE_WEB_INTERFACE "Enable web interface" FALSE)
option(ENABLE_PLOTTER "Enable plotter" FALSE)
option(ENABLE_COLLECT "Enable collect" FALSE)
option(ENABLE_SCRIPT_INTERFACE "Enable script interface" FALSE)
option(ENABLE_EXPERIMENTAL "Enable experimental mode" OFF)

if (ENABLE_BENCHMARKS)
   message(STATUS "Benchmarks enabled")
   configure_file(${CMAKE_SOURCE_DIR}/benchmarks.sqlite3 ${CMAKE_BINARY_DIR}/benchmarks.sqlite3 COPYONLY)
   add_subdirectory(benchmarks)
endif()

if (ENABLE_WEB_INTERFACE)
   message(STATUS "Wt Web interface is enabled")
   add_subdirectory(portal)
endif()   

if (ENABLE_DATABASE_SERIALIZER)
   message(STATUS "Wt DBO database serializer is enabled")
   add_subdirectory(database-serializer)
endif()

if (ENABLE_VIEWER)
   message(STATUS "OpenGL SDL system viewer is enabled")
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

## TODO
# if (ENABLE_FUZZING)
#    message(STATUS "Fuzzing is enabled")
#    add_subdirectory(fuzz_tests)
# endif()  

if (ENABLE_TESTS)
   include(CTest)
   enable_testing()
   add_subdirectory(tests)
endif()