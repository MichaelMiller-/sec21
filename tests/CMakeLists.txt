find_package(Boost 1.66 REQUIRED)
# find_package(ViennaCL)
   # VIENNACL_WITH_OPENCLbasti
   # VIENNACL_WITH_CUDA

## TODO
## find_package(Catch2 REQUIRED)
## target_link_libraries(tests Catch2::Catch2)

if (OPENMP_FOUND)
   message("-- ViennaCL OpenMP Support is enabled") ## TODO: for "exe name"
endif()

function(test test)
   string(REPLACE "/" "_" tmp ${test})
   add_executable(test.${tmp} "${CMAKE_CURRENT_LIST_DIR}/${test}.cpp")

   ## boost 
   target_include_directories(test.${tmp} PRIVATE ${Boost_INCLUDE_DIRS})
   target_link_libraries(test.${tmp} PRIVATE ${Boost_LIBRARIES})
   ## range-v3
   target_include_directories(test.${tmp} PRIVATE "${CMAKE_SOURCE_DIR}/deps/range-v3/include")
   ## ned14/outcome/outcome.hpp -> use this until boost 1.70 is available
   target_include_directories(test.${tmp} PRIVATE "${CMAKE_SOURCE_DIR}/deps/ned14-outcome")
   ## nlohman/json.hpp
   target_include_directories(test.${tmp} PRIVATE "${CMAKE_SOURCE_DIR}/deps")
   ## viennacl
   target_include_directories(test.${tmp} PRIVATE "${CMAKE_SOURCE_DIR}/deps/viennacl")

   # if (OPENMP_FOUND)
   #    # target_compile_definitions(test.${tmp} PRIVATE VIENNACL_WITH_OPENMP)
   #    # target_compile_options(test.${tmp} PRIVATE "${OpenMP_CXX_FLAGS}")
   #    # target_link_libraries(test.${tmp} PRIVATE "${OpenMP_CXX_FLAGS}")
   # endif()

   if (ENABLE_BENCHMARKS)
      add_compile_definitions(CATCH_CONFIG_ENABLE_BENCHMARKING)
   endif()

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

      add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)

      target_link_libraries(test.${tmp} PRIVATE "-ltbb")
      # target_link_libraries(test.${tmp} PRIVATE "-lpthread")
      target_link_libraries(test.${tmp} PRIVATE "-lstdc++fs")

     if (ENABLE_COVERAGE)
        target_link_libraries(test.${tmp} "--coverage")
     endif()   
   endif()

   target_link_libraries(test.${tmp} PUBLIC sec21)

   add_test(test.${tmp} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.${tmp}")
endfunction()

## configure_file(in out COPYONLY)

## unit tests via catch (alphabetical order)
##
test("cache")
test("combine")
test("concat")
test("contains")
test("dispatcher")
test("double_values")
test("drawdown")
test("enumerate")
test("fixed_string")
test("flat_vector")
test("in_range")
test("lookup")
test("make_iota_array")
test("math")
test("perfect.forwarding")
test("projection_func")
test("property_tree")
test("serializer")
test("state_machine")
test("strong_types")
test("strong_pair")
test("structural_analysis.axis")
test("structural_analysis.force")
test("structural_analysis.member")
test("structural_analysis.support")
test("traits")
test("units")
test("units.dimension")
test("units.quantity")
test("units.ratio")

## TODO 
## set_tests_properties("units" PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=/path_where_libbaz.so_is_on_my_target_system")