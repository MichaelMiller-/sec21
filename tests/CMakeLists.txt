find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

include_directories("${CMAKE_SOURCE_DIR}/deps/range-v3/include")

function(test test)
   string(REPLACE "/" "_" tmp ${test})
   add_executable(test.${tmp} ${CMAKE_CURRENT_LIST_DIR}/${test}.cpp)

   include(CheckCXXCompilerFlag)

   if (MSVC_VERSION GREATER_EQUAL "1900")
      CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
      if (_cpp_latest_flag_supported)
         add_compile_options("/std:c++latest")
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
   endif()

   if (MSVC)
      add_compile_options(/W4) ## /WX)
   else()
      add_compile_options(-Wall -Wextra) ## -Wpedantic)
   endif()

   add_test(test.${tmp} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.${tmp})

   # target_link_libraries(test.${tmp} -llua)
   # target_link_libraries(test.${tmp} sqlpp11-connector-sqlite3)

   if (ENABLE_COVERAGE)
      if (NOT MSVC)
         target_link_libraries(test.${tmp} --coverage)
      endif()
   endif()   
endfunction()

## unit tests via catch (alphabetical order)
##
test("cache")
test("concat")
test("dispatcher")
test("double_values")
test("enumerate")
test("fixed_string")
test("in_range")
test("make_iota_array")
test("strong_types")
test("strong_pair")
test("structural_analysis.support")
test("units")

