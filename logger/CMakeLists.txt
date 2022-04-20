project(logger VERSION 0.0.1 LANGUAGES CXX)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in "${PROJECT_BINARY_DIR}/config.h")

add_library(${PROJECT_NAME} STATIC
        src/${PROJECT_NAME}.cpp
        src/console_logger.h
        src/console_logger.cpp
        src/database_logger.h
        src/database_logger.cpp
        src/file_logger.h
        src/file_logger.cpp
        src/string_loger.h
        src/string_loger.cpp
        )

target_include_directories(${PROJECT_NAME}
        PUBLIC "include"
        PRIVATE ${PROJECT_BINARY_DIR}
        PRIVATE "include/sec21"
        PRIVATE "src")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

find_package(fmt REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE sec21)

include(CTest)
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif ()

install(TARGETS ${PROJECT_NAME} ARCHIVE)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})