find_package(Catch2 CONFIG REQUIRED)
find_package(Boost 1.70 REQUIRED)
find_package(OpenMP REQUIRED)

include(ExternalProject)
ExternalProject_Add(
   ViennaCL-tests
   PREFIX tests
   URL https://sourceforge.net/projects/viennacl/files/1.7.x/ViennaCL-1.7.1.zip
   CMAKE_ARGS -D BUILD_EXAMPLES=OFF
      -D ENABLE_OPENCL=OFF
      -D CMAKE_BUILD_TYPE=Release
      -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/viennacl
)

## speed up the compiletime process
add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PRIVATE Catch2::Catch2)
add_dependencies(catch_main ViennaCL-tests)

function(test test)
   string(REPLACE "/" "_" tmp ${test})
   add_executable(test.${tmp} "${CMAKE_CURRENT_LIST_DIR}/${test}.cpp")

   target_link_libraries(test.${tmp} PRIVATE Catch2::Catch2)
   # boost
   target_include_directories(test.${tmp} PRIVATE ${Boost_INCLUDE_DIRS})
   target_link_libraries(test.${tmp} PRIVATE ${Boost_LIBRARIES})
   # openmp
   target_link_libraries(test.${tmp} PRIVATE OpenMP::OpenMP_CXX)
   target_compile_definitions(test.${tmp} PRIVATE VIENNACL_WITH_OPENMP)
   ## viennacl
   target_include_directories(test.${tmp} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/viennacl/include")

   if (ENABLE_BENCHMARKS)
      add_compile_definitions(CATCH_CONFIG_ENABLE_BENCHMARKING)
   endif()

   include(CheckCXXCompilerFlag)
   ## platform specific
   ##
   if (MSVC_VERSION GREATER_EQUAL "1900")
      CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
      if (_cpp_latest_flag_supported)
         target_compile_options(test.${tmp} PRIVATE "/std:c++latest")
      endif()
      target_compile_options(test.${tmp} PRIVATE "/permissive-")
   endif()
   
   if (CMAKE_COMPILER_IS_GNUCXX)
      CHECK_CXX_COMPILER_FLAG("-fconcepts" _cpp_concepts_supported)
      if (_cpp_concepts_supported)
         target_compile_options(test.${tmp} PRIVATE "-fconcepts")
      endif()
   endif()

   if (CMAKE_COMPILER_IS_GNUCXX)
      CHECK_CXX_COMPILER_FLAG("--fcoroutines-ts:" _cpp_coroutines_supported)

      target_link_libraries(test.${tmp} PRIVATE "-lstdc++fs")

      # -msse4.2
      # -mavx
      # -mavx2
      if (ENABLE_EXPERIMENTAL)
         add_compile_options("-march=sandybridge")
      endif()
   endif()

   target_link_libraries(test.${tmp} PRIVATE catch_main)
   target_link_libraries(test.${tmp} PUBLIC sec21)
   
   foreach(ARG ${ARGN})
      string(LENGTH "${ARG}" length_data_file)
      if(length_data_file)
         message(STATUS "copy configure file: ${PROJECT_SOURCE_DIR}/data/${ARG}")
         configure_file("${PROJECT_SOURCE_DIR}/data/${ARG}" "${PROJECT_BINARY_DIR}/${ARG}" COPYONLY)
      endif()
   endforeach()   

   add_test(
      NAME test.${tmp} 
      COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.${tmp}"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endfunction()

## copy optional data 
if(EXISTS "${PROJECT_SOURCE_DIR}/data/cp_results.sh")
   configure_file("${PROJECT_SOURCE_DIR}/data/cp_results.sh" "${PROJECT_BINARY_DIR}/cp_results.sh" COPYONLY)
endif()

## unit tests via catch (alphabetical order)
##
test("access")
test("any_of")
test("all_of")
test("cache")
test("concat")
test("contains")
test("dispatcher")
test("double_values")
test("drawdown")
test("enclose")
test("enumerate")
test("erase_if_index")
test("features")
test("fixed_string")
test("flat_matrix")
test("flat_vector")
test("for_each_chunk")
test("for_each_indexed")
test("in_range")
test("make_iota_array")
test("math")
test("numeric.make_vector")
test("perfect.forwarding")
test("projection_func")
test("property_tree")
test("resource")
test("serializer")
test("structural_analysis.axis")
# test("structural_analysis.beam")
test("structural_analysis.force")
test("structural_analysis.geometry")
test("structural_analysis.loadcase")
test("structural_analysis.lookup")
test("structural_analysis.member")
test("structural_analysis.node")
test("structural_analysis.space_truss" "example_1.json")
test("structural_analysis.space_truss.example1.temperature" "example_1.json")
test("structural_analysis.space_truss.example2" "example_2.json" "example_2_load.json")
test("structural_analysis.space_truss.example2.temperature" "example_2.json" "example_2_temperature_load.json")
test("structural_analysis.space_truss.example3" "example_3.json" "example_3_load.json")
test("structural_analysis.space_truss.example4" "example_4.json" "example_4_load.json")
test("structural_analysis.space_truss.example5" "example_5.json" "example_5_load.json")
test("structural_analysis.space_truss.example6" "example_6.json" "example_6_load.json")
test("structural_analysis.space_truss.example7" "example_7.json" "example_7_load.json")
test("structural_analysis.space_truss.member")
test("structural_analysis.space_truss.node")
test("structural_analysis.support")
test("traits")
test("units.dimension")
test("units.quantity" "test_data_units.json")
test("units.ratio")
test("zip")
