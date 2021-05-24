find_package(Catch2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

function(test filename)
   project(test.editor.${filename})
   add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/${filename}.cpp")

   target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

   target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
   target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2)
   target_link_libraries(${PROJECT_NAME} PUBLIC sec21)
   
   add_test(
      NAME ${PROJECT_NAME}
      COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endfunction()

## unit tests via catch (alphabetical order)
##
test("command.add_member")
test("command.add_node")
test("command.add_support")
test("command.loadfile")
