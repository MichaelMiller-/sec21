## https://google.github.io/oss-fuzz/
## https://blog.hboeck.de/archives/868-How-Heartbleed-couldve-been-found.html
##

function(fuzz filename)
    set(fuzz_project_name fuzz.${filename})
    add_executable(${fuzz_project_name} ${CMAKE_CURRENT_SOURCE_DIR}/${filename}.cpp)

    target_compile_features(${fuzz_project_name} PRIVATE cxx_std_20)

    target_compile_options(${fuzz_project_name} PRIVATE -fsanitize=fuzzer,undefined,address)

    target_link_libraries(${fuzz_project_name} PRIVATE -coverage -fsanitize=fuzzer,undefined,address)
    target_link_libraries(${fuzz_project_name} PRIVATE enable_compiler_warnings)

    # Allow short runs during automated testing to see if something new breaks
    set(FUZZ_RUNTIME
            10  # Default of 10 seconds
            CACHE STRING "Number of seconds to run fuzz tests during CTest run"
            )

    add_test(
            NAME ${fuzz_project_name}
            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${fuzz_project_name} -max_total_time=${FUZZ_RUNTIME})
endfunction()

## Fuzzer only works with Clang yet
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    fuzz(broken_parser)
    fuzz(simple)
endif()