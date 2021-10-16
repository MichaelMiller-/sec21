project(http_connection VERSION 0.0.1 LANGUAGES CXX)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in "${PROJECT_BINARY_DIR}/config.h")

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(${PROJECT_NAME} STATIC src/${PROJECT_NAME}.cpp)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC "include"
        PUBLIC ${PROJECT_BINARY_DIR}
        PRIVATE "src")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

find_package(cpprestsdk CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal)
target_link_libraries(${PROJECT_NAME} PRIVATE
        "$<$<CXX_COMPILER_ID:Clang,GNU>:cpprestsdk::cpprestsdk_boost_internal;cpprestsdk::cpprestsdk_openssl_internal>"
        "$<$<CXX_COMPILER_ID:MSVC>:cpprestsdk::cpprestsdk_brotli_internal>"
)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json nlohmann_json::nlohmann_json)

