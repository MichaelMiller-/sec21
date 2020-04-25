
find_package(Catch2 CONFIG REQUIRED)
find_package(Boost 1.70 REQUIRED)
# TODO:
# find_package(ViennaCL)

# if (OPENMP_FOUND AND VIENNACL_FOUND)
#    message(STATUS "ViennaCL OpenMP Support is enabled") ## TODO: for "exe name"
# endif()


function(test test)
   string(REPLACE "/" "_" tmp ${test})
   add_executable(test.${tmp} "${CMAKE_CURRENT_LIST_DIR}/${test}.cpp")

   target_link_libraries(test.${tmp} PRIVATE Catch2::Catch2)
   # boost
   target_include_directories(test.${tmp} PRIVATE ${Boost_INCLUDE_DIRS})
   target_link_libraries(test.${tmp} PRIVATE ${Boost_LIBRARIES})
   ## viennacl
   target_include_directories(test.${tmp} PRIVATE "${CMAKE_SOURCE_DIR}/deps/viennacl")

   if (ENABLE_BENCHMARKS)
      add_compile_definitions(CATCH_CONFIG_ENABLE_BENCHMARKING)
   endif()

   include(CheckCXXCompilerFlag)

   ## platform specific
   ##
   if (MSVC_VERSION GREATER_EQUAL "1900")
      CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
      if (_cpp_latest_flag_supported)
         add_compile_options("/std:c++latest")
      endif()

      add_compile_options("/permissive-")
   endif()

   if (CMAKE_COMPILER_IS_GNUCXX)
      CHECK_CXX_COMPILER_FLAG("-std=c++2a" _cpp20_experimental_supported)
      if (_cpp20_experimental_supported)
         add_compile_options("-std=c++2a")

         CHECK_CXX_COMPILER_FLAG("-fconcepts" _cpp_concepts_supported)
         if (_cpp_concepts_supported)
            add_compile_options("-fconcepts")
         endif()
      endif()

      target_link_libraries(test.${tmp} PRIVATE "-lstdc++fs")

      # -msse4.2
      # -mavx
      # -mavx2
      if (ENABLE_EXPERIMENTAL)
         add_compile_options("-march=sandybridge")
      endif()
   endif()

   target_link_libraries(test.${tmp} PUBLIC sec21)

   add_test(
      NAME test.${tmp} 
      COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.${tmp}"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endfunction()

## test data
configure_file("${PROJECT_SOURCE_DIR}/data/example_1.json" "${PROJECT_BINARY_DIR}/example_1.json" COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/data/example_2.json" "${PROJECT_BINARY_DIR}/example_2.json" COPYONLY)

## unit tests via catch (alphabetical order)
##
test("any_of")
test("all_of")
test("cache")
test("concat")
test("contains")
test("dispatcher")
test("double_values")
test("drawdown")
test("enumerate")
test("fixed_string")
test("flat_matrix")
test("flat_vector")
test("for_each_chunk")
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
test("structural_analysis.force")
test("structural_analysis.lookup")
test("structural_analysis.member")
test("structural_analysis.space_truss")
#  test("structural_analysis.space_truss.example2")
test("structural_analysis.space_truss.member")
test("structural_analysis.space_truss.node")
test("structural_analysis.support")
test("traits")
test("units.dimension")
test("units.quantity")
test("units.ratio")
test("zip")
