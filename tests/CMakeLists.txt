find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

## TODO
## find_package(Catch2 REQUIRED)
## target_link_libraries(tests Catch2::Catch2)

function(test test)
   string(REPLACE "/" "_" tmp ${test})
   add_executable(test.${tmp} "${CMAKE_CURRENT_LIST_DIR}/${test}.cpp")

   target_include_directories(test.${tmp} PRIVATE "${CMAKE_SOURCE_DIR}/deps/range-v3/include")
   ## ned14/outcome.hpp -> use this until boost 1.70 is available
   target_include_directories(test.${tmp} PRIVATE "${CMAKE_SOURCE_DIR}/deps/ned14")
   ## nlohman/json.hpp
   target_include_directories(test.${tmp} PRIVATE "${CMAKE_SOURCE_DIR}/deps")
   ## viennacl
   target_include_directories(test.${tmp} PRIVATE "${CMAKE_SOURCE_DIR}/deps/viennacl")

   if (OPENMP_FOUND)
      message("-- ViennaCL OpenMP Support is enabled") ## TODO: for "exe name"
      target_compile_definitions(test.${tmp} PRIVATE VIENNACL_WITH_OPENMP)
      target_compile_options(test.${tmp} PRIVATE "${OpenMP_CXX_FLAGS}")
      target_link_libraries(test.${tmp} PRIVATE "${OpenMP_CXX_FLAGS}")
   endif()

   # VIENNACL_WITH_OPENCL
   # VIENNACL_WITH_CUDA

   include(CheckCXXCompilerFlag)

   if (MSVC_VERSION GREATER_EQUAL "1900")
      CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
      if (_cpp_latest_flag_supported)
         add_compile_options("/std:c++latest")
      endif()

      add_compile_options("/permissive-")
      add_compile_options(/W4) ## /WX)

      ## TODO check this option
      if (OPENMP_FOUND)
         add_compile_options("/Zc:twoPhase-")
      endif()
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

      add_compile_options(-Wall -Wextra -Wpedantic)

      target_link_libraries(test.${tmp} "-lstdc++fs")
   endif()

   add_test(test.${tmp} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.${tmp}")

   if (ENABLE_COVERAGE)
      if (NOT MSVC)
         target_link_libraries(test.${tmp} "--coverage")
      endif()
   endif()   
endfunction()

## configure_file(in out COPYONLY)

## unit tests via catch (alphabetical order)
##
test("cache")
test("combine")
test("concat")
test("dispatcher")
test("double_values")
test("enumerate")
test("fixed_string")
test("flat_vector")
test("in_range")
test("make_iota_array")
test("math")
test("perfect.forwarding")
test("projection_func")
test("property_tree")
test("serializer")
test("strong_types")
test("strong_pair")
test("structural_analysis.force")
test("structural_analysis.member")
test("structural_analysis.node")
test("structural_analysis.support")
test("traits")
test("units")