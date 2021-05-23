include(ExternalProject)
ExternalProject_Add(
   ViennaCL-benchmark-solver-backend
   URL https://sourceforge.net/projects/viennacl/files/1.7.x/ViennaCL-1.7.1.zip
   CMAKE_ARGS -Wno-dev -D BUILD_EXAMPLES=OFF
      -D ENABLE_OPENCL=OFF
      -D CMAKE_BUILD_TYPE=Release
      -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/viennacl
)

project(benchmark.structural_analysis.space_struss.solver.backend)
add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/structural_analysis.space_struss.solver.backend.cpp")

configure_file("${CMAKE_SOURCE_DIR}/benchmarks/boilerplate.h.in" "${PROJECT_BINARY_DIR}/generated_boilerplate.h")
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

## viennacl
target_compile_definitions(${PROJECT_NAME} PRIVATE VIENNACL_WITH_OPENMP)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/viennacl/include")
add_dependencies(${PROJECT_NAME} ViennaCL-benchmark-solver-backend)

find_package(nonius CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Nonius::nonius)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
## disable non MPL2 compatible features, or in other words disable the features which are still under the LGPL.
target_compile_definitions(${PROJECT_NAME} PRIVATE EIGEN_MPL2_ONLY)
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)
target_link_libraries(${PROJECT_NAME} PRIVATE sec21)
