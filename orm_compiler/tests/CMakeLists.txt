find_package(Catch2 CONFIG REQUIRED)
find_package(Boost 1.70 REQUIRED)
find_package(fmt REQUIRED)

## speed up the compiletime process
# add_library(catch_main STATIC catch_main.cpp)
# target_link_libraries(catch_main PRIVATE Catch2::Catch2)

function(test filename)
   project(test.${filename})
   add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/${filename}.cpp")

   target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

   target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2)
   target_link_libraries(${PROJECT_NAME} INTERFACE Boost::boost)
   target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
   #target_link_libraries(${PROJECT_NAME} PRIVATE catch_main)
   
   add_test(
      NAME ${PROJECT_NAME} 
      COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endfunction()

test("parse_columns")
