cmake_minimum_required(VERSION 3.1)
project(benchmark.expression_templates VERSION 0.0.1 LANGUAGES CXX)

add_executable(${PROJECT_NAME} expression_templates.cpp)

configure_file("${CMAKE_SOURCE_DIR}/benchmarks/boilerplate.h.in" "${PROJECT_BINARY_DIR}/generated_boilerplate.h")
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

find_package(nonius CONFIG REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Nonius::nonius)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
