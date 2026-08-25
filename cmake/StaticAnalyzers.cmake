# option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
# option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
# option(ENABLE_GCC_ANALYZER "Enable static analysis with gcc" OFF)

find_program(CPPCHECK cppcheck)
## TODO write a function with ARG as ignore list
if (CPPCHECK)
    message(STATUS "'cppcheck' is available. run with: make cppcheck")
    add_custom_target(
            cppcheck
            COMMAND ${CPPCHECK}
            --enable=warning,performance,portability,information
            --std=c++17
            --library=qt.cfg
            --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
            --verbose
            --quiet
            -i_build -ibuild -iviewer/implot_demo.cpp -iimplot_internal.h
            ${PROJECT_SOURCE_DIR})
endif ()

## TODO
#if(ENABLE_CLANG_TIDY)
# -fanalyzer -Wanalyzer-too-complex
# -fanalyzer-verbosity=4
# -fdump-analyzer-state-purge
# -fdump-analyzer-supergraph
## #endif()

## TODO
# if(ENABLE_GCC_ANALYZER)
#   message(SEND_ERROR "TODO")
# # TODO
# # -fanalyzer -Wanalyzer-too-complex
# # -fanalyzer-verbosity=4
# # -fdump-analyzer-state-purge
# # -fdump-analyzer-supergraph
# endif()
