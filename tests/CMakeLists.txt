find_package(Catch 2.1.2 REQUIRED)
include_directories(${CATCH_INCLUDE_DIRS})

function(test test)
    string(REPLACE "/" "_" tmp ${test})
    
	add_executable(test.${tmp} ${CMAKE_CURRENT_LIST_DIR}/${test}.cpp)
    add_test(test.${tmp} test.${tmp})
endfunction()

## unit tests via catch
##
test(concat)
# test(structural_analysis_framework)
# # test(structural_analysis_force)
# test(ranges)
# test(triangulate)
# test(matrix)
# test(model)
# test(task.single_span_beam)

## Installation
## Copies files from build directory into install directory.
# file(GLOB executables "${GLOBAL_OUTPUT_PATH}/*.exe")
# install(FILES ${executables} DESTINATION "bin")

## TODO: possible to pass arguments to catch_runner
## add_test(task.single_span_beam25 task.single_span_beam 25)
## set_tests_properties(task.single_span_beam25 PROPERTIES PASS_REGULAR_EXPRESSION "25 is 5")