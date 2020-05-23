find_package(Catch2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

function(test test)
   string(REPLACE "/" "_" tmp ${test})
   add_executable(test.${tmp} "${CMAKE_CURRENT_LIST_DIR}/${test}.cpp")

   target_link_libraries(test.${tmp} PRIVATE Catch2::Catch2)
   target_link_libraries(test.${tmp} PRIVATE glm)
   target_link_libraries(test.${tmp} PUBLIC sec21)
   
   add_test(
      NAME test.${tmp} 
      COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.${tmp}"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endfunction()

## unit tests via catch (alphabetical order)
##
test("intersection")
