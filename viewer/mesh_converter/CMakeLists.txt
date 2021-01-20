cmake_minimum_required(VERSION 3.1)
project(mesh_converter VERSION 0.0.1 LANGUAGES CXX)

configure_file("${PROJECT_SOURCE_DIR}/config.h.in" "${PROJECT_BINARY_DIR}/config.h")

add_executable(${PROJECT_NAME} "mesh_converter.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.70 COMPONENTS program_options REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED) 
find_package(assimp CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)
target_link_libraries(${PROJECT_NAME} PUBLIC sec21)
