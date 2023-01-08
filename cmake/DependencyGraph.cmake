## Custom target: Generates a project dependency graph
##
add_custom_target(
        dependency_graph
        COMMAND ${CMAKE_COMMAND} "--graphviz=${PROJECT_NAME}-dependency-graph.dot" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Generate dependency graph: ${PROJECT_NAME}-dependency-graph.dot"
        VERBATIM)

message(STATUS "Custom target to generate a project dependency graph. Run with: make dependency_graph")
