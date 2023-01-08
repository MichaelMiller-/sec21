cmake_minimum_required(VERSION 3.9)
project(sec21
        VERSION 0.0.1
        DESCRIPTION "sec21::workspace"
        )

include(GNUInstallDirs)
include(CTest)
## TODO: CDash integration: page 316

## Make CMake modules available
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(StandardProjectSettings)
include(DependencyGraph)

## header-only library
add_library(${PROJECT_NAME} INTERFACE)
## alias
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

## project dependencies
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json nlohmann_json::nlohmann_json)

include(CompilerWarnings)
set_project_warnings(${PROJECT_NAME})

include(Doxygen)
enable_doxygen()

include(StaticAnalyzers)

option(ENABLE_VIEWER "Enable system viewer" OFF)
option(ENABLE_DEVTOOLS "tools needed for the development process" OFF)
option(ENABLE_PRODUCTION "not listed on GITHUB" OFF)
option(ENABLE_VIENNA_CL "Enable ViennaCL solver" OFF)

if (ENABLE_VIEWER)
    message(STATUS "OpenGL system viewer is enabled")
    add_subdirectory(viewer)
endif ()

if (BUILD_TESTING) # option is created by CTest
    enable_testing()
    add_subdirectory(tests)
endif ()

##
##
add_subdirectory(http_connection)
add_subdirectory(make_readme)

option(ENABLE_WORK_IN_PROGRESS "work in progress!" OFF)
if (ENABLE_WORK_IN_PROGRESS)
    # TODO: delete: add_subdirectory(editor)
    # add_subdirectory(fuzzing_test)
    add_subdirectory(integration_test_sqlite3_backend)
    add_subdirectory(opengl_window)
    add_subdirectory(plotter)
    add_subdirectory(pmr)
    add_subdirectory(polymorphism)
    # add_subdirectory(portal) ## Wt
    add_subdirectory(que)
    add_subdirectory(runtime_enviroment)
    add_subdirectory(translate)
    add_subdirectory(udf)
    add_subdirectory(vulkan)
    # add_subdirectory(viewer_sfml)
    add_subdirectory(worker) ## test logger
    # add_subdirectory(workspace) ## Alia test
endif ()


## Installation + CMake config files
##
include(CMakePackageConfigHelpers)

install(TARGETS ${PROJECT_NAME}
        EXPORT local-export-name
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

install(EXPORT local-export-name
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        NAMESPACE ${PROJECT_NAME}::
        FILE ${PROJECT_NAME}Config.cmake
        )

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        )
