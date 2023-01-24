include(CMakeFindDependencyMacro)

find_dependency(nlohmann_json 3.10.0)

include(${CMAKE_CURRENT_LIST_DIR}/sec21Targets.cmake)