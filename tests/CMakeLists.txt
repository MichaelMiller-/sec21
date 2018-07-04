find_path(CATCH_INCLUDE_DIR catch.hpp)
include_directories(${CATCH_INCLUDE_DIR})

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

function(test test)
   string(REPLACE "/" "_" tmp ${test})
      
   add_executable(test.${tmp} ${CMAKE_CURRENT_LIST_DIR}/${test}.cpp)
   add_test(test.${tmp} test.${tmp})
endfunction()

## unit tests via catch
##
test("concat")
test("dispatcher")
test("cache")
test("in_range")

