find_package(Catch2 CONFIG REQUIRED)
find_package(Boost 1.70 REQUIRED)
find_package(OpenMP REQUIRED)

include(ExternalProject)
ExternalProject_Add(
   ViennaCL-tests
   URL https://sourceforge.net/projects/viennacl/files/1.7.x/ViennaCL-1.7.1.zip
   CMAKE_ARGS -Wno-dev -D BUILD_EXAMPLES=OFF
      -D ENABLE_OPENCL=OFF
      -D CMAKE_BUILD_TYPE=Release
      -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/viennacl
)

## speed up the compiletime process
add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PRIVATE Catch2::Catch2)
add_dependencies(catch_main ViennaCL-tests)

function(test filename)
   project(test.${filename})
   add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/${filename}.cpp")

   target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

   target_link_libraries(${PROJECT_NAME} INTERFACE Boost::boost)
   target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
   ## viennacl
   target_compile_definitions(${PROJECT_NAME} PRIVATE VIENNACL_WITH_OPENMP)
   target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/viennacl/include")

   target_link_libraries(${PROJECT_NAME} PRIVATE catch_main)
   target_link_libraries(${PROJECT_NAME} PUBLIC sec21)
   
   foreach(ARG ${ARGN})
      string(LENGTH "${ARG}" length_data_file)
      if(length_data_file)
         message(STATUS "copy configure file: ${CMAKE_SOURCE_DIR}/data/${ARG}")
         configure_file("${CMAKE_SOURCE_DIR}/data/${ARG}" "${PROJECT_BINARY_DIR}/${ARG}" COPYONLY)
      endif()
   endforeach()   

   add_test(
      NAME ${PROJECT_NAME} 
      COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endfunction()

## copy optional data 
if(EXISTS "${CMAKE_SOURCE_DIR}/data/cp_results.sh")
   configure_file("${CMAKE_SOURCE_DIR}/data/cp_results.sh" "${PROJECT_BINARY_DIR}/cp_results.sh" COPYONLY)
endif()

## unit tests via catch (alphabetical order)
##
test("access")
test("any_of")
test("all_of")
test("bit_pattern")
test("cache")
test("concat")
test("contains")
test("database.query")
test("dispatcher")
test("double_values")
test("drawdown")
test("enclose")
test("enumerate")
test("erase_if_index")
test("event.input_manager")
test("features")
test("flat_vector")
test("for_each_chunk")
test("for_each_indexed")
test("in_range")
test("is_specialized")
test("make_iota_array")
test("math")
test("min_absent")
test("numeric.drop")
test("numeric.flatten")
test("numeric.make_matrix")
test("numeric.make_vector")
test("perfect.forwarding")
test("projection_func")
test("property_tree")
test("resource")
test("row_col")
test("scope_guard")
test("structural_analysis.axis")
test("structural_analysis.common")
test("structural_analysis.force")
test("structural_analysis.geometry")
test("structural_analysis.loadcase" "example_1.json")
test("structural_analysis.member")
test("structural_analysis.node")
test("structural_analysis.space_truss")
test("structural_analysis.space_truss.example1.half_load" "example_1.json")
test("structural_analysis.space_truss.example1.fileloader" "example_1.json")
test("structural_analysis.space_truss.example1.temperature" "example_1.json")
test("structural_analysis.space_truss.example1.order" "example_1.json")
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
test("validate")
test("zip")

## WORK IN PROGRESS
# test("indexed_character")
# test("dictionary")
# test("chaining")
# test("log")