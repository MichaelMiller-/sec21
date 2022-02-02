cmake_minimum_required(VERSION 3.8)
project(make_readme VERSION 0.1.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h @ONLY)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
