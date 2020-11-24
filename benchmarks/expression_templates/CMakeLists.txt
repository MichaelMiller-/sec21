cmake_minimum_required(VERSION 3.1)

set (CMAKE_CXX_STANDARD 20)

project(expression_templates)

add_executable(${PROJECT_NAME} expression_templates.cpp)

find_package(nonius CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Nonius::nonius)