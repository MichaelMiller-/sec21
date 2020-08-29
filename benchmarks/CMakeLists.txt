project(benchmarks)

set("${PROJECT_NAME}_VERSION_MAJOR" 0)
set("${PROJECT_NAME}_VERSION_MINOR" 1)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in "${PROJECT_BINARY_DIR}/config.h")

include_directories(${PROJECT_BINARY_DIR})

set(SOURCES benchmarks.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

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

target_compile_options(${PROJECT_NAME} PRIVATE "-O3")

include(ExternalProject)
ExternalProject_Add(
   ViennaCL-benchmarks
   URL https://sourceforge.net/projects/viennacl/files/1.7.x/ViennaCL-1.7.1.zip
   CMAKE_ARGS -Wno-dev -D BUILD_EXAMPLES=OFF
      -D ENABLE_OPENCL=OFF
      -D CMAKE_BUILD_TYPE=Release
      -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/viennacl
)
add_dependencies(${PROJECT_NAME} ViennaCL-benchmarks)

# target_include_directories(test.${tmp} PRIVATE ${DebViennaCL_INCLUDE_DIRS})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.70 COMPONENTS program_options REQUIRED)
find_package(spdlog REQUIRED)
find_package(OpenMP REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenCL)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
# target_link_libraries(${PROJECT_NAME} PRIVATE DebViennaCL::DebViennaCL)

# target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL::OpenCL)
# target_compile_definitions(${PROJECT_NAME} PRIVATE VIENNACL_WITH_OPENCL)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/viennacl/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE VIENNACL_WITH_OPENMP)

## FindCUDAToolkit

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PUBLIC sec21)
