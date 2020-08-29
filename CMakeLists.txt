## TODO: OpenMP Support is improved if cmake version is greater then 3.9
cmake_minimum_required(VERSION 3.1)

project(sec21 CXX)
include(cmake/StandardProjectSettings.cmake)

set(${PROJECT_NAME}_VERSION_MAJOR 1 CACHE STRING "major version" FORCE)
set(${PROJECT_NAME}_VERSION_MINOR 2 CACHE STRING "minor version" FORCE)
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR} CACHE STRING "version" FORCE)


set(tool_dest "bin")
set(include_dest "include/${PROJECT_NAME}-${sec21_VERSION}")
set(main_lib_dest "lib/${PROJECT_NAME}-${sec21_VERSION}")
# set(lib_dest ${main_lib_dest}/${CMAKE_BUILD_TYPE}")
set(header_path "${PROJECT_SOURCE_DIR}/include")

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

## dependencies 
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  include(cmake/Vcpkg.cmake)
  ## all dependencies
  ## TODO: reduce dependencies <-> install packages only if cmake options are set
  vcpkg_install(nlohmann-json boost-mp11 boost-math boost-qvm boost-outcome boost-ublas catch2 glm spdlog sdl2 glew imgui entt assimp)
  include(${CMAKE_TOOLCHAIN_FILE})
endif()

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

include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME})

include(cmake/Sanitizers.cmake)
enable_sanitizers(${PROJECT_NAME})

include(cmake/Doxygen.cmake)
enable_doxygen()

include(cmake/StaticAnalyzers.cmake)

configure_file("${PROJECT_SOURCE_DIR}/include/sec21/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h")

install(FILES cmake/${PROJECT_NAME}-config.cmake DESTINATION ${main_lib_dest})
install(FILES ${header_files} DESTINATION ${include_dest})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.h DESTINATION ${include_dest})

option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
   target_precompile_headers(sec21 INTERFACE <vector> <string> <map> <utility> <algorithm> <numeric>)
endif()

option(ENABLE_TESTS "Enable tests" ON)
option(ENABLE_BENCHMARKS "Enable benchmarks" OFF)
option(ENABLE_DATABASE_SERIALIZER "Enable database serlializer" OFF)
option(ENABLE_VIEWER "Enable system viewer" OFF)
option(ENABLE_WEB_INTERFACE "Enable web interface" OFF)
option(ENABLE_PLOTTER "Enable plotter" OFF)
option(ENABLE_COLLECT "Enable collect" OFF)
option(ENABLE_SCRIPT_INTERFACE "Enable script interface" OFF)
option(ENABLE_EXPERIMENTAL "Enable experimental mode" OFF)

if (ENABLE_BENCHMARKS)
   message(STATUS "Benchmarks enabled")
   configure_file("${CMAKE_HOME_DIRECTORY}/data/benchmark_example.json" "${PROJECT_BINARY_DIR}/benchmark_example.json" COPYONLY)
   configure_file("${CMAKE_HOME_DIRECTORY}/data/benchmark_example_load.json" "${PROJECT_BINARY_DIR}/benchmark_example_load.json" COPYONLY)
   add_subdirectory(benchmarks)
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

## TODO: view Release build type { -fno-rtti }
## TODO: consider https://gcc.gnu.org/gcc-10/changes.html

set(tool_dest "bin")
set(include_dest "include/my_library-${MY_LIBRARY_VERSION}")
set(main_lib_dest "lib/my_library-${MY_LIBRARY_VERSION}")
