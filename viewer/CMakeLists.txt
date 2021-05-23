cmake_minimum_required(VERSION 3.1)

project(viewer)

set("${PROJECT_NAME}_VERSION_MAJOR" 0)
set("${PROJECT_NAME}_VERSION_MINOR" 1)

configure_file("${PROJECT_SOURCE_DIR}/config.h.in" "${PROJECT_BINARY_DIR}/config.h" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/camera.json" "${PROJECT_BINARY_DIR}/camera.json" COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/settings.json" "${PROJECT_BINARY_DIR}/settings.json" COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/shader1.vs" "${PROJECT_BINARY_DIR}/shader1.vs" COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/shader1.fs" "${PROJECT_BINARY_DIR}/shader1.fs" COPYONLY)
# configure_file("${PROJECT_SOURCE_DIR}/data/text.vs" "${PROJECT_BINARY_DIR}/text.vs" COPYONLY)
# configure_file("${PROJECT_SOURCE_DIR}/data/text.fs" "${PROJECT_BINARY_DIR}/text.fs" COPYONLY)
# configure_file("${PROJECT_SOURCE_DIR}/data/arial.ttf" "${PROJECT_BINARY_DIR}/arial.ttf" COPYONLY)
configure_file("${CMAKE_HOME_DIRECTORY}/data/example_1.json" "${PROJECT_BINARY_DIR}/example_1.json" COPYONLY)
configure_file("${CMAKE_HOME_DIRECTORY}/data/example_2.json" "${PROJECT_BINARY_DIR}/example_2.json" COPYONLY)
configure_file("${CMAKE_HOME_DIRECTORY}/data/example_3.json" "${PROJECT_BINARY_DIR}/example_3.json" COPYONLY)
configure_file("${CMAKE_HOME_DIRECTORY}/data/example_4.json" "${PROJECT_BINARY_DIR}/example_4.json" COPYONLY)
configure_file("${CMAKE_HOME_DIRECTORY}/data/example_5.json" "${PROJECT_BINARY_DIR}/example_5.json" COPYONLY)
configure_file("${CMAKE_HOME_DIRECTORY}/data/example_6.json" "${PROJECT_BINARY_DIR}/example_6.json" COPYONLY)
configure_file("${CMAKE_HOME_DIRECTORY}/data/example_7.json" "${PROJECT_BINARY_DIR}/example_7.json" COPYONLY)
configure_file("${CMAKE_HOME_DIRECTORY}/data/example_7_load.json" "${PROJECT_BINARY_DIR}/example_7_load.json" COPYONLY)
# configure_file("${CMAKE_HOME_DIRECTORY}/data/example_1_result.json" "${PROJECT_BINARY_DIR}/example_1_result.json" COPYONLY)
# configure_file("${CMAKE_HOME_DIRECTORY}/data/example_3_result.json" "${PROJECT_BINARY_DIR}/example_3_result.json" COPYONLY)
# configure_file("${CMAKE_HOME_DIRECTORY}/data/example_4_result.json" "${PROJECT_BINARY_DIR}/example_4_result.json" COPYONLY)
# configure_file("${CMAKE_HOME_DIRECTORY}/data/example_5_result.json" "${PROJECT_BINARY_DIR}/example_5_result.json" COPYONLY)
# configure_file("${CMAKE_HOME_DIRECTORY}/data/example_6_result.json" "${PROJECT_BINARY_DIR}/example_6_result.json" COPYONLY)
# configure_file("${CMAKE_HOME_DIRECTORY}/data/example_7_result.json" "${PROJECT_BINARY_DIR}/example_7_result.json" COPYONLY)

set(IMGUI_SOURCES "imgui_impl/imgui_impl_sdl.h" "imgui_impl/imgui_impl_sdl.cpp" "imgui_impl/imgui_impl_opengl3.h" "imgui_impl/imgui_impl_opengl3.cpp")
set(SOURCES "viewer.cpp" ${IMGUI_SOURCES})

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}")

include(CheckCXXCompilerFlag)
if (MSVC_VERSION GREATER_EQUAL "1900")
   CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
   if (_cpp_latest_flag_supported)
      target_compile_options(${PROJECT_NAME} PRIVATE "/std:c++latest")
   endif()
   target_compile_options(${PROJECT_NAME} PRIVATE "/permissive-")
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
   CHECK_CXX_COMPILER_FLAG("-fconcepts" _cpp_concepts_supported)
   if (_cpp_concepts_supported)
      target_compile_options(${PROJECT_NAME} PRIVATE "-fconcepts")
   endif()
endif()

find_package(SDL2 CONFIG REQUIRED) 
# find_package(sdl2-image CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED) 
find_package(nlohmann_json CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
# find_package(freetype CONFIG REQUIRED)

target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLEW)
# target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_FORCE_MESSAGES)

target_link_libraries(${PROJECT_NAME} PRIVATE "GL")
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static)
# target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_image)
# target_link_libraries(${PROJECT_NAME} PRIVATE freetype)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
## todo: link imgui targets -> ImGui::ImplSDL2 & ImGui::ImplOpenGL3
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

include(ExternalProject)
ExternalProject_Add(
   BoostSML
   GIT_REPOSITORY https://github.com/boost-ext/sml
   CMAKE_ARGS -Wno-dev 
      -D SML_BUILD_BENCHMARKS=OFF
      -D SML_BUILD_DOCS=OFF
      -D SML_BUILD_EXAMPLES=OFF 
      -D SML_BUILD_TESTS=OFF 
      -D CMAKE_BUILD_TYPE=Release
      -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/boostsml
)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/boostsml/include")

target_link_libraries(${PROJECT_NAME} PUBLIC sec21)

## todo: CPack

## 
add_subdirectory(mesh_converter)

if (ENABLE_TESTS)
   include(CTest)
   enable_testing()
   add_subdirectory(tests)
endif()
