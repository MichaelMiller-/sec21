find_path(CATCH_INCLUDE_DIR catch.hpp)
include_directories(${CATCH_INCLUDE_DIR})

# find_package(Boost)
# include_directories(${Boost_INCLUDE_DIRS})

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

## Installation
## Copies files from build directory into install directory.
# file(GLOB executables "${GLOBAL_OUTPUT_PATH}/*.exe")
# install(FILES ${executables} DESTINATION "bin")

## TODO: possible to pass arguments to catch_runner
## add_test(task.single_span_beam25 task.single_span_beam 25)
## set_tests_properties(task.single_span_beam25 PROPERTIES PASS_REGULAR_EXPRESSION "25 is 5")