project(editor.backend VERSION 0.0.1 LANGUAGES CXX)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in "${PROJECT_BINARY_DIR}/config.h")

set(SOURCES editor.backend.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR})
## c++17 is required by boost::beast
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

find_package(Boost REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PUBLIC sec21)

if (ENABLE_TESTS)
   add_subdirectory(tests)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME)
