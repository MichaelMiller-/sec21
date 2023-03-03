cmake_minimum_required(VERSION 3.23)
project(sec21
        VERSION 1.0.0
        DESCRIPTION "sec21::workspace"
        )

include(CTest)
## TODO: CDash integration: page 316

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/StandardProjectSettings.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DependencyGraph.cmake)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(enable_compiler_warnings INTERFACE)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerWarnings.cmake)
set_project_warnings(enable_compiler_warnings)

## Header-only library
add_library(sec21 INTERFACE)
## Alias
add_library(sec21::sec21 ALIAS sec21)

target_compile_features(sec21 INTERFACE cxx_std_20)

## TODO use target_sources() with FILE_SET -> requires cmake > 3.23
## set(HEADER_FILES )
## target_sources(sec21 INTERFACE FILE_SET HEADERS)

target_include_directories(sec21 INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

## TODO:
## target_link_libraries(sec21 INTERFACE enable_compiler_warnings)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerWarnings.cmake)
set_project_warnings(sec21)

if (BUILD_TESTING) # option is created by CTest
    enable_testing()
    add_subdirectory(tests)
endif ()

option(ENABLE_FUZZING "Enable fuzzing test builds" OFF)
if(ENABLE_FUZZING)
    message("Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
    add_subdirectory(fuzzing)
endif()

## Internal projects
##
add_subdirectory(make_readme)

## Installation + CMake config files
##
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS sec21
        EXPORT sec21ExportTarget
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

install(EXPORT sec21ExportTarget
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        NAMESPACE ${PROJECT_NAME}::
        FILE sec21Config.cmake
        )

write_basic_package_version_file(
        sec21ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
        )

install(FILES
        sec21Config.cmake
        ${PROJECT_BINARY_DIR}/sec21ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        )
