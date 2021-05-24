cmake_minimum_required(VERSION 3.1)
project(viewer VERSION 0.0.1 LANGUAGES CXX)


configure_file(${PROJECT_SOURCE_DIR}/config.h.in "${PROJECT_BINARY_DIR}/config.h" @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/data/camera.json "${PROJECT_BINARY_DIR}/camera.json" COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/data/settings.json "${PROJECT_BINARY_DIR}/settings.json" COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/data/shader1.vs "${PROJECT_BINARY_DIR}/shader1.vs" COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/data/shader1.fs "${PROJECT_BINARY_DIR}/shader1.fs" COPYONLY)
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
# set(IMPLOT_SOURCES "implot.cpp"  "implot_demo.cpp"  "implot.h"  "implot_internal.h"  "implot_items.cpp")
set(SOURCES "viewer.cpp" ${IMGUI_SOURCES} ${IMPLOT_SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

find_package(SDL2 CONFIG REQUIRED) 
# find_package(sdl2-image CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED) 
find_package(nlohmann_json CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
# find_package(freetype CONFIG REQUIRED)

# find_package(libigl CONFIG REQUIRED)
# target_link_libraries(main PRIVATE igl::core igl::common igl::opengl igl::opengl_glfw)



target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_FORCE_SIZE_T_LENGTH)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:GLM_FORCE_MESSAGES>)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:GLM_FORCE_INLINE>)

# GLM_FORCE_SSE2, GLM_FORCE_SSE3, GLM_FORCE_SSSE3, GLM_FORCE_SSE41, GLM_FORCE_SSE42, GLM_FORCE_AVX, GLM_FORCE_AVX2 or GLM_FORCE_AVX512.
# GLM_FORCE_EXPLICIT_CTOR

target_link_libraries(${PROJECT_NAME} PRIVATE "GL")
target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static)
# target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_image)
# target_link_libraries(${PROJECT_NAME} PRIVATE freetype)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE sec21)
target_link_libraries(${PROJECT_NAME} PRIVATE http_connection)

## 
if (ENABLE_DEVTOOLS)
   message(STATUS "mesh_converter is enabled")
   add_subdirectory(mesh_converter)
endif()   

if (ENABLE_TESTS)
   include(CTest)
   enable_testing()
   add_subdirectory(tests)
endif()

## 
install(TARGETS ${PROJECT_NAME} RUNTIME)