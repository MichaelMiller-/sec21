configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(make_readme)

target_sources(make_readme PRIVATE make_readme.cpp)

target_include_directories(make_readme PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_compile_features(make_readme PUBLIC cxx_std_20)

## dependencies
##
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(make_readme PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(make_readme PRIVATE fmt::fmt-header-only)
target_link_libraries(make_readme PRIVATE enable_compiler_warnings)