cmake_minimum_required(VERSION 3.9)
project(sec21
        VERSION 0.0.1
        DESCRIPTION "sec21::workspace"
        )

include(GNUInstallDirs)
include(CTest)
## TODO: CDash integration: page 316

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/StandardProjectSettings.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DependencyGraph.cmake)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(enable_compiler_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(enable_compiler_warnings)

## Header-only library
add_library(${PROJECT_NAME} INTERFACE)
## Alias
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

## TODO use target_sources() with FILE_SET -> requires cmake > 3.23
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

## Project dependencies
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json nlohmann_json::nlohmann_json)
## TODO: target_link_libraries(${PROJECT_NAME} INTERFACE enable_compiler_warnings)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME})

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
include(CMakePackageConfigHelpers)

install(TARGETS ${PROJECT_NAME}
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
