find_package(Catch2 CONFIG REQUIRED)

function(test filename)
    project(test.logger.${filename})
    add_executable(${PROJECT_NAME} ${filename}.cpp)

    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

    target_include_directories(${PROJECT_NAME} PRIVATE "../include/sec21")

    target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2)
    target_link_libraries(${PROJECT_NAME} PRIVATE sec21)

    add_test(
            NAME ${PROJECT_NAME}
            COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}"
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endfunction()

## unit tests via catch (alphabetical order)
##
test("entry")

