cmake_minimum_required(VERSION 3.1)

project(benchmark.expression_templates)

add_executable(${PROJECT_NAME} expression_templates.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

find_package(nonius CONFIG REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Nonius::nonius)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
