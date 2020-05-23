## TODO: OpenMP Support is improved if cmake version is greater then 3.9
cmake_minimum_required(VERSION 3.1)

project(sec21 CXX)
include(cmake/StandardProjectSettings.cmake)

## dependencies 
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  include(cmake/Vcpkg.cmake)
  ## all dependencies
  ## TODO: reduce dependencies <-> cmake options
  vcpkg_install(nlohmann-json boost catch2 glm spdlog sdl2 glew imgui entt assimp)
  include(${CMAKE_TOOLCHAIN_FILE})
endif()

## header-only library
add_library(${PROJECT_NAME} INTERFACE)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.70 REQUIRED)
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

option(ENABLE_PCH "Enable Precompiled Headers" ON)
if (ENABLE_PCH)
   target_precompile_headers(sec21 INTERFACE <vector> <string> <map> <utility> <algorithm>)
endif()

option(ENABLE_TESTS "Enable tests" ON)
option(ENABLE_BENCHMARKS "Enable benchmarks" OFF)
option(ENABLE_DATABASE_SERIALIZER "Enable database serlializer" OFF)
option(ENABLE_VIEWER "Enable system viewer" ON)
option(ENABLE_WEB_INTERFACE "Enable web interface" OFF)
option(ENABLE_PLOTTER "Enable plotter" OFF)
option(ENABLE_COLLECT "Enable collect" OFF)
option(ENABLE_SCRIPT_INTERFACE "Enable script interface" OFF)
option(ENABLE_EXPERIMENTAL "Enable experimental mode" OFF)

if (ENABLE_BENCHMARKS)
   message(STATUS "Benchmarks enabled")
   ## configure_file(${CMAKE_SOURCE_DIR}/benchmarks.sqlite3 ${CMAKE_BINARY_DIR}/benchmarks.sqlite3 COPYONLY)
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

## copy optional data 
if(EXISTS "${PROJECT_SOURCE_DIR}/data/cp_gen.sh")
   configure_file("${PROJECT_SOURCE_DIR}/data/cp_gen.sh" "${PROJECT_BINARY_DIR}/cp_gen.sh" COPYONLY)
endif()
