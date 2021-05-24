cmake_minimum_required(VERSION 3.9)
project(sec21 
   VERSION 0.0.1
   DESCRIPTION "sec21::workspace"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SEC21_CMAKE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
   message(STATUS "Emscpriten build")
   add_subdirectory(workspace)
   # add_subdirectory(webviewer)
else()   

include(${SEC21_CMAKE_PATH}/StandardProjectSettings.cmake)

## header-only library
add_library(${PROJECT_NAME} INTERFACE)

# TODO
#if(NOT TARGET sec21::sec21)
  #include(${SEC21_CMAKE_PATH}/sec21Targets.cmake)
#endif()

# target_compile_definitions(${PROJECT_NAME}
#    INTERFACE
#   PRIVATE $<$<COMPILE_LANG_AND_ID:CXX,AppleClang,Clang>:COMPILING_CXX_WITH_CLANG>
#           $<$<COMPILE_LANG_AND_ID:CXX,Intel>:COMPILING_CXX_WITH_INTEL>
#           $<$<COMPILE_LANG_AND_ID:CXX,GNU>:COMPILING_CXX_WITH_GNU>
# )

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)

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

if (CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
   message(ERROR "IWYU is found")
endif()

if (CMAKE_CXX_LINK_LIBRARY_FILE_FLAG)
   message(ERROR "llff is found")
endif()
# find_program(CMAKE_CXX_INCLUDE_WHAT_YOU_USE NAMES iwyu)
# if (CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
#    message(STATUS "*include what you use* is enabled")
# endif()

# find_program(CMAKE_LINK_WHAT_YOU_USE NAMES lwyu)
# if (CMAKE_LINK_WHAT_YOU_USE)
#    message(STATUS "*link what you use* is enabled")
# endif()


## SEC21_PURE
# target_compile_definitions();

## TODO replace with c++ modules
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
   target_precompile_headers(${PROJECT_NAME} INTERFACE <vector> <string> <map> <utility> <algorithm> <numeric>)
endif()

option(ENABLE_TESTS "Enable tests" ON)
option(ENABLE_EDITOR "Enable web editor" OFF)
option(ENABLE_BENCHMARKS "Enable benchmarks" OFF)
option(ENABLE_VIEWER "Enable system viewer" OFF)
option(ENABLE_WEB_INTERFACE "Enable web interface" OFF)
option(ENABLE_PLOTTER "Enable plotter" OFF)
option(ENABLE_COLLECT "Enable collect" OFF)
option(ENABLE_SCRIPT_INTERFACE "Enable script interface" OFF)
option(ENABLE_EXPERIMENTAL "Enable experimental mode" OFF)
option(ENABLE_DEVTOOLS "tools needed for the development process" OFF)

if (ENABLE_BENCHMARKS)
   message(STATUS "Benchmarks enabled")
   add_subdirectory(benchmarks)
endif()

if (ENABLE_WEB_INTERFACE)
   message(STATUS "Wt Web interface is enabled")
   add_subdirectory(portal)
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

if (ENABLE_EDITOR)
   message(STATUS "Webeditor is enabled")
   add_subdirectory(editor)
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



include(CheckLanguage)
check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
enable_language(CUDA)
lse()
message(STATUS "No CUDA compiler found")
endif()

add_subdirectory(orm_compiler)

## WORK IN PROGRESS
# add_subdirectory(integration_test_sqlite3_backend)
# add_subdirectory(worker)
# add_subdirectory(http_connection)
# add_subdirectory(controller)
# add_subdirectory(resttest)
# add_subdirectory(runtime_enviroment)
# add_subdirectory(que)
# add_subdirectory(esbp)
# add_subdirectory(udf)
# add_subdirectory(pmr)
# add_subdirectory(polymorphism)
# add_subdirectory(worker)
# add_subdirectory(kraken)
# add_subdirectory(allocator_awareness)
# add_subdirectory(translate)
# add_subdirectory(webapp)


## 
##
add_custom_target(
   ## TODO: name schema for custom targets
   dependency_graph
   COMMAND ${CMAKE_COMMAND} "--graphviz=${PROJECT_NAME}-dependency-graph.dot" .
   WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
   COMMENT "Generate dependency graph: ${PROJECT_NAME}-dependency-graph.dot"
   VERBATIM)

## installation
##
# TODO: investigate: include(GNUInstallDirs)
install(TARGETS ${CMAKE_PROJECT_NAME}
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib)

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
# objdump -p bin/rte | grep NEEDED
#   NEEDED               libpthread.so.0
#   NEEDED               libstdc++.so.6
#   NEEDED               libm.so.6
#   NEEDED               libgcc_s.so.1
#   NEEDED               libc.so.6
#   NEEDED               ld-linux-x86-64.so.2
# set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libgcc1 (>= 1:3.4.2-12)")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.33)")

## PKGBUILD 
##
# configure_file(${PROJECT_SOURCE_DIR}/PKGBUILD.in ${PROJECT_BINARY_DIR}/PKGBUILD.generated @ONLY)
# configure_file(${PROJECT_SOURCE_DIR}/generate_pkgbuild.sh.in ${PROJECT_BINARY_DIR}/generate_pkgbuild.sh @ONLY)

# add_custom_command(
#    OUTPUT "package.checksum"
#    COMMAND ${CMAKE_COMMAND} -E sha512sum "${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-Linux.tar.gz" > package.checksum.generated
#    COMMAND cut --delimiter=\" \" -f 1 package.checksum.generated > package.checksum
#    COMMAND sed -e "s/GENERATED_HASH_LIST/sdfsd\$\(cat package.checksum\)/" PKGBUILD.generated > PKGBUILD
#    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
#    COMMENT "Write package checksum"
# )

# add_custom_command(
#    OUTPUT VERSION
#    COMMAND ${CMAKE_COMMAND} -E echo "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" > VERSION
#    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
#    COMMENT "Write version file")
# install(FILES "${PROJECT_BINARY_DIR}/VERSION" DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CPack)

add_custom_target(
   packages
   ## todo: code duplication -> solve with dependencies
   COMMAND ${CMAKE_COMMAND} -E echo "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" > VERSION
   COMMAND cpack
   ## run makepkg for ArchLinux
   COMMAND sh generate_pkgbuild.sh
   COMMAND makepkg -f
   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
   VERBATIM
   COMMENT "Make installation packages"
)
## -C "Release" -G "STGZ;TGZ"
## cpack -"C Release -G STGZ;TGZ;DEB"

##
## TODO: copy dockerfile 
# add_custom_target(
#    docker
# )

## copy optional data
## 
if(EXISTS "${PROJECT_SOURCE_DIR}/data/cp_gen.sh")
   configure_file(${PROJECT_SOURCE_DIR}/data/cp_gen.sh ${PROJECT_BINARY_DIR}/cp_gen.sh COPYONLY)
endif()

endif()
