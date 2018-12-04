find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/deps/Range-V3-VS2015/include)

function(test test)
   string(REPLACE "/" "_" tmp ${test})
      
   add_executable(test.${tmp} ${CMAKE_CURRENT_LIST_DIR}/${test}.cpp)

   if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
   endif()

   if (NOT MSVC)
      add_compile_options("-std=c++17")
   endif()

   if (MSVC)
      add_compile_options(/W4)
   else()
      add_compile_options(-Wall -Wextra -Wpedantic)
   endif()

   add_test(test.${tmp} test.${tmp})

   if (NOT MSVC)
      target_link_libraries(test.${tmp} --coverage)
   endif()
endfunction()

## unit tests via catch
##
test("concat")
test("dispatcher")
test("cache")
test("in_range")
test("strong_types")
test("units")
