cmake_minimum_required(VERSION 3.9)
project(sec21
        VERSION 0.0.1
        DESCRIPTION "sec21::workspace"
        )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SEC21_CMAKE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(${SEC21_CMAKE_PATH}/StandardProjectSettings.cmake)

## header-only library
add_library(${PROJECT_NAME} INTERFACE)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.70 REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} INTERFACE Boost::boost)
target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json nlohmann_json::nlohmann_json)
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE include)

include(${SEC21_CMAKE_PATH}/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME})

include(${SEC21_CMAKE_PATH}/Sanitizers.cmake)
enable_sanitizers(${PROJECT_NAME})

include(${SEC21_CMAKE_PATH}/Doxygen.cmake)
enable_doxygen()

include(${SEC21_CMAKE_PATH}/StaticAnalyzers.cmake)

## https://blog.kitware.com/static-checks-with-cmake-cdash-iwyu-clang-tidy-lwyu-cpplint-and-cppcheck/
if (CMAKE_LINK_WHAT_YOU_USE)
    message(STATUS "*link what you use* is enabled")
endif ()

## TODO replace with c++ modules
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
    target_precompile_headers(${PROJECT_NAME} INTERFACE <vector> <string> <map> <utility> <algorithm> <numeric>)
endif ()

option(ENABLE_BENCHMARKS "Enable benchmarks" OFF)
option(ENABLE_VIEWER "Enable system viewer" OFF)
option(ENABLE_SCRIPT_INTERFACE "Enable script interface" OFF)
option(ENABLE_TESTS "Enable tests" ON)
option(ENABLE_DEVTOOLS "tools needed for the development process" OFF)
option(ENABLE_PRODUCTION "not listed on GITHUB" OFF)

if (ENABLE_BENCHMARKS)
    message(STATUS "Benchmarks enabled")
    add_subdirectory(benchmarks)
endif ()

if (ENABLE_VIEWER)
    message(STATUS "OpenGL system viewer is enabled")
    add_subdirectory(viewer)
endif ()

if (ENABLE_SCRIPT_INTERFACE)
    message(STATUS "Lua script interface is enabled")
    add_subdirectory(scripting)
endif ()

if (ENABLE_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif ()

##
##
add_custom_target(
        dependency_graph
        COMMAND ${CMAKE_COMMAND} "--graphviz=${PROJECT_NAME}-dependency-graph.dot" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Generate dependency graph: ${PROJECT_NAME}-dependency-graph.dot"
        VERBATIM)

##
##
add_subdirectory(http_connection)
add_subdirectory(orm_compiler)

## production code - not listed on GITHUB
##
if (ENABLE_PRODUCTION)
    add_subdirectory(codegen)
    add_subdirectory(collect)
    add_subdirectory(controller)
    add_subdirectory(database_connection)
    add_subdirectory(editor)
    add_subdirectory(esbp)
    # add_subdirectory(fuzzing_test)
    # add_subdirectory(integration_test_sqlite3_backend)
    add_subdirectory(log_service)
    add_subdirectory(logger)
    add_subdirectory(opengl_window)
    add_subdirectory(plotter)
    add_subdirectory(pmr)
    add_subdirectory(polymorphism)
    # add_subdirectory(portal) ## Wt
    add_subdirectory(que)
    add_subdirectory(resttest)
    add_subdirectory(runtime_enviroment)
    add_subdirectory(translate)
    add_subdirectory(udf)
    add_subdirectory(viewer_sfml)
    # add_subdirectory(webviewer) ## Emscripten test
    add_subdirectory(worker)
    # add_subdirectory(workspace) ## Alia test
endif ()

## installation
##
include(GNUInstallDirs)
## set different paths
#install(TARGETS ${CMAKE_PROJECT_NAME}
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Planungsbüro Michael Miller")
set(CPACK_VERBATIM_VARIABLES true)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "info@pb-miller.de")

## rpm
##
## https://nebula-graph.io/posts/packaging-with-cpack-in-nebula-graph/
## TODO: set(CMAKE_RPM_PACKAGE_GROUP ${PROJECT_NAME})
# set(CMAKE_RPM_PACKAGE_LICENSE "TODO")

## dep
##
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
## TODO: automate with:
# objdump -p bin/esbp | grep NEEDED
# objdump -p bin/controller | grep NEEDED
# NEEDED               libpthread.so.0
# NEEDED               libstdc++.so.6
# NEEDED               libgcc_s.so.1
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31), libpqxx-6.4 (>= 6.4)")

## PKGBUILD
##
#configure_file(${PROJECT_SOURCE_DIR}/VERSION.in ${PROJECT_BINARY_DIR}/VERSION @ONLY)
#configure_file(${PROJECT_SOURCE_DIR}/PKGBUILD.in ${PROJECT_BINARY_DIR}/PKGBUILD.generated @ONLY)
#configure_file(${PROJECT_SOURCE_DIR}/generate_pkgbuild.sh.in ${PROJECT_BINARY_DIR}/generate_pkgbuild.sh @ONLY)

# add_custom_command(
#    OUTPUT "package.checksum"
#    COMMAND ${CMAKE_COMMAND} -E sha512sum "${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-Linux.tar.gz" > package.checksum.generated
#    COMMAND cut --delimiter=\" \" -f 1 package.checksum.generated > package.checksum
#    COMMAND sed -e "s/GENERATED_HASH_LIST/sdfsd\$\(cat package.checksum\)/" PKGBUILD.generated > PKGBUILD
#    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
#    COMMENT "Write package checksum"
# )

include(CPack)

add_custom_target(
        packages
        COMMAND cpack -C Release -G "STGZ;TGZ;DEB"
        ## run makepkg for ArchLinux
        #COMMAND sh generate_pkgbuild.sh
        #COMMAND makepkg -f
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        VERBATIM
        COMMENT "Make installation packages"
)

##
## TODO: copy dockerfile
# add_custom_target(
#    docker
# )

## copy optional data
##
if (EXISTS "${PROJECT_SOURCE_DIR}/data/cp_gen.sh")
    configure_file(${PROJECT_SOURCE_DIR}/data/cp_gen.sh ${PROJECT_BINARY_DIR}/cp_gen.sh COPYONLY)
endif ()

## TODO: take a look at "lizard"
## https://www.youtube.com/watch?v=IndGlm2uZCU

## TODD: inspect translation unit
## https://www.youtube.com/watch?v=3KoXeegncrs&list=PLHTh1InhhwT5o3GwbFYy3sR7HDNRA353e&index=2
## g++ -std=c++20 -E main.cpp | egrep -v '#' | tee main.i
