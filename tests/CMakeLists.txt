find_package(Catch2 CONFIG REQUIRED)
find_package(Boost 1.70 REQUIRED)
find_package(fmt REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

## speed up the compiletime process
add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2 Catch2::Catch2WithMain)

if (ENABLE_VIENNA_CL)
    include(ExternalProject)
    ExternalProject_Add(
            ViennaCL-tests
            URL https://sourceforge.net/projects/viennacl/files/1.7.x/ViennaCL-1.7.1.zip
            CMAKE_ARGS -Wno-dev
            -D BUILD_EXAMPLES=OFF
            -D ENABLE_OPENCL=OFF
            -D CMAKE_BUILD_TYPE=Release
            -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/viennacl
    )
    add_dependencies(catch_main ViennaCL-tests)

    find_package(OpenMP REQUIRED)
endif ()

find_package(Coroutines)

function(test filename)
    project(test.${filename})
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/${filename}.cpp)

    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
    ## needed for Clang
    ## TODO: fails
    # target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CXX_COMPILER_ID:Clang>:-fcoroutines>")

    target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)
    target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
    ## disable non MPL2 compatible features, or in other words disable the features which are still under the LGPL.
    target_compile_definitions(${PROJECT_NAME} PRIVATE EIGEN_MPL2_ONLY)
    target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)

    if (Coroutines_FOUND)
        target_link_libraries(${PROJECT_NAME} PRIVATE std::coroutines)
    endif ()

    if (ENABLE_VIENNA_CL)
        target_compile_definitions(${PROJECT_NAME} PRIVATE VIENNACL_WITH_OPENMP)
        target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/viennacl/include)
        target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
    endif ()

    target_link_libraries(${PROJECT_NAME} PRIVATE catch_main)
    target_link_libraries(${PROJECT_NAME} PRIVATE sec21)

    foreach (ARG ${ARGN})
        string(LENGTH "${ARG}" length_data_file)
        if (length_data_file)
            message(STATUS "copy configure file: ${CMAKE_SOURCE_DIR}/data/${ARG}")
            ## needed for console "make test"
            configure_file(${CMAKE_SOURCE_DIR}/data/${ARG} "${PROJECT_BINARY_DIR}/${ARG}" COPYONLY)
            ## needed for direct call from build directory
            configure_file(${CMAKE_SOURCE_DIR}/data/${ARG} "${CMAKE_BINARY_DIR}/${ARG}" COPYONLY)
            ## needed for Clion
            configure_file(${CMAKE_SOURCE_DIR}/data/${ARG} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ARG}" COPYONLY)
        endif ()
    endforeach ()

    add_test(
            NAME ${PROJECT_NAME}
            COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}"
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endfunction()

## copy optional data 
if (EXISTS "${CMAKE_SOURCE_DIR}/data/cp_results.sh")
    configure_file("${CMAKE_SOURCE_DIR}/data/cp_results.sh" "${PROJECT_BINARY_DIR}/cp_results.sh" COPYONLY)
endif ()

## unit tests via catch (alphabetical order)
##
test("aabb")
test("all_of")
test("alphabet")
test("any_of")
test("arity")
test("bit_pattern")
test("concat")
# test("curry")
# test("database.query")
test("debug_timer")
test("dispatcher")
test("double_values")
test("drawdown")
test("enumerate")
test("enum_to_string")
test("erase_if_index")
# test("event.input_manager")
test("flat_vector")
test("for_each_chunk")
test("for_each_indexed")
test("in_range")
test("make_iota_array")
test("math")
test("min_absent")
test("numeric.drop")
test("numeric.flatten")
test("numeric.make_matrix")
test("numeric.make_vector")
test("polygon")
test("projection_func")
test("resource")
test("scope_guard")
test("strong_type")
test("timestamp")
test("traits")
test("type_traits.has_type")
test("type_traits.is_specialized")
test("units.dimension")
test("units.quantity" "test_data_units.json")
test("units.ratio")
test("validate")
test("zip")


if (ENABLE_WORK_IN_PROGRESS)
    test("chaining")
    test("coroutines")
    test("demux")
    test("dictionary")
    test("dimap")
    test("disk_space")
    test("entity_manager")
    test("features.concepts")
    # test("features.concurrency")
    test("features")
    test("features.iterators")
    test("features.perfect_forwarding")
    test("features.type_traits")
    test("geometry")
    test("literal_template")
    test("log")
    test("material")
    test("pipeable")
    test("pipe")
    test("process")
    test("quadradic_matrix_plotter")
    test("ranges")
    # test("reflection")
    test("replay_manager")
    # test("sat_sudoku")
    test("serializer")
    test("snake_ladders")
    test("strange_heap")
    # test("structural_analysis.beam")
    # test("structural_analysis.dlt")
    test("structural_analysis.validation")
    test("synchronized")
    test("system_reader")
    # test("tasks")
    test("test_framework")
    # test("thread_shared_state")
    test("structural_analysis.common")
    test("structural_analysis.loadcase" "example_1.json")
    test("structural_analysis.space_truss")
    test("structural_analysis.space_truss.example2" "example_2.json" "example_2_load.json")
    test("structural_analysis.space_truss.example3" "example_3.json" "example_3_load.json")
    test("structural_analysis.space_truss.example4" "example_4.json" "example_4_load.json")
    test("structural_analysis.space_truss.example5" "example_5.json" "example_5_load.json")

    if (ENABLE_VIENNA_CL)
        test("structural_analysis.space_truss.solver.backend.viennacl")
    endif ()
    test("structural_analysis.space_truss.solver.backend.eigen")
endif ()


