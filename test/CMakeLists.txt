include_directories(${CMAKE_CURRENT_LIST_DIR})

function(test test)
    string(REPLACE "/" "_" tmp ${test})
    add_executable(test.${tmp} ${CMAKE_CURRENT_LIST_DIR}/${test}.cpp)
    add_test(test.${tmp} ${CMAKE_BINARY_DIR}/test.${tmp})
endfunction()

## test(structural_analysis_framework)
## test(structural_analysis_force)
test(ranges)


