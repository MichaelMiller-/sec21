cmake_minimum_required(VERSION 3.25)

## Options
##
option(SEC21_ENABLE_COVERAGE "Enable coverage build" OFF)
option(SEC21_ENABLE_FUZZING "Enable fuzzing tests" OFF)
option(BUILD_TESTING "Build tests" ON)

## VCPKG_MANIFEST_FEATURES must be configured before project(), because the
## vcpkg toolchain is evaluated while project() is executed.
##
if (BUILD_TESTING) # option is also used by CTest
    list(APPEND VCPKG_MANIFEST_FEATURES "testing")
endif()

## Project
##
include(${CMAKE_CURRENT_LIST_DIR}/projectVersion.cmake)
project(sec21
        VERSION ${projectVersion}
        DESCRIPTION "sec21::utilities"
        LANGUAGES CXX
)

## CMake modules
##
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CompilerWarnings)

## Header-only library
##
add_library(sec21 INTERFACE)
## Alias
add_library(sec21::sec21 ALIAS sec21)

target_compile_features(sec21 INTERFACE cxx_std_26)

target_include_directories(sec21 INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_sources(sec21 INTERFACE
        FILE_SET HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            # database
            include/sec21/database/column_type.h
            include/sec21/database/constraints.h
            include/sec21/database/data_mapper.h
            include/sec21/database/query.h
            # literals
            include/sec21/literals/memory.h
            # meta
            include/sec21/meta/contains.h
            include/sec21/meta/difference.h
            include/sec21/meta/intersection.h
            include/sec21/meta/merge.h
            include/sec21/meta/push_front.h
            include/sec21/meta/remove_if.h
            include/sec21/meta/sort.h
            include/sec21/meta/type_list.h
            include/sec21/meta/zip_transform.h
            # policy
            include/sec21/policy/addition.h
            include/sec21/policy/compare.h
            include/sec21/policy/compare_less.h
            include/sec21/policy/decrement.h
            include/sec21/policy/division.h
            include/sec21/policy/increment_alphabet.h
            include/sec21/policy/increment.h
            include/sec21/policy/integer_arithmetic.h
            include/sec21/policy/multiplication.h
            include/sec21/policy/print.h
            include/sec21/policy/subtraction.h
            # reflection
            include/sec21/reflection/column.h
            include/sec21/reflection/column_type.h
            include/sec21/reflection/get_column.h
            include/sec21/reflection/table.h
            # type_traits
            include/sec21/type_traits/contains.h
            include/sec21/type_traits/index_of.h
            include/sec21/type_traits/is_instance_of.h
            include/sec21/type_traits/is_specialized.h
            include/sec21/type_traits/is_std_string.h
            include/sec21/type_traits/is_tuple.h
            include/sec21/type_traits/member_pointer_traits.h
            # units/dimensions
            include/sec21/units/dimensions/acceleration.h
            include/sec21/units/dimensions/area.h
            include/sec21/units/dimensions/base_dimensions.h
            include/sec21/units/dimensions/current.h
            include/sec21/units/dimensions/electric_charge.h
            include/sec21/units/dimensions/energy.h
            include/sec21/units/dimensions/force.h
            include/sec21/units/dimensions/frequency.h
            include/sec21/units/dimensions/luminous_intensity.h
            include/sec21/units/dimensions/mass.h
            include/sec21/units/dimensions/power.h
            include/sec21/units/dimensions/pressure.h
            include/sec21/units/dimensions/second_moment_of_area.h
            include/sec21/units/dimensions/substance.h
            include/sec21/units/dimensions/temperature.h
            include/sec21/units/dimensions/time.h
            include/sec21/units/dimensions/velocity.h
            include/sec21/units/dimensions/voltage.h
            include/sec21/units/dimensions/volume.h
            # units
            include/sec21/units/concepts.h
            include/sec21/units/dimension.h
            include/sec21/units/quantity.h
            include/sec21/units/ratio.h
            include/sec21/units/unit.h
            include/sec21/units/unit_casting.h
            #
            include/sec21/all_of.h
            include/sec21/alphabet.h
            include/sec21/any_of.h
            include/sec21/arity.h
            include/sec21/array_qvm_vec_traits.h
            include/sec21/bit_pattern.h
            include/sec21/blob.h
            include/sec21/concat.h
            include/sec21/database
            include/sec21/expects.h
            include/sec21/fixed_string.h
            include/sec21/for_each_adjacent.h
            include/sec21/for_each_chunk.h
            include/sec21/for_each_indexed.h
            include/sec21/json_serializer_array.h
            include/sec21/json_serializer_optional.h
            include/sec21/json_serializer_uuid.h
            include/sec21/json_serializer_variant.h
            include/sec21/limited_quantity.h
            include/sec21/literals
            include/sec21/memory.h
            include/sec21/overloaded.h
            include/sec21/policy
            include/sec21/reflection
            include/sec21/scope_guard.h
            include/sec21/split_if.h
            include/sec21/strong_type.h
            include/sec21/to_array.h
            include/sec21/transform.h
            include/sec21/transform_if.h
            include/sec21/type_traits
            include/sec21/units
            include/sec21/units.h
            include/sec21/validate.h
            include/sec21/write_csv.h
            include/sec21/zip.h
)

## Link to this 'library' in order to use the warnings specified in CompilerWarnings.cmake
##
include(CompilerWarnings)
add_library(sec21_enable_compiler_warnings INTERFACE)
add_library(sec21::enable_compiler_warnings ALIAS sec21_enable_compiler_warnings)
enable_warnings(sec21_enable_compiler_warnings)

## Testing
##
if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()

## Fuzzing
##
if (SEC21_ENABLE_FUZZING)
    message("Building Fuzz Tests, using fuzzing sanitizer https://www.llvm.org/docs/LibFuzzer.html")
    add_subdirectory(fuzzing)
endif ()

## Installation
##
install(
        TARGETS sec21
        EXPORT sec21Targets
        FILE_SET HEADERS
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT sec21Targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        NAMESPACE ${PROJECT_NAME}::
        FILE sec21Targets.cmake
)

## Package configuration
##
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sec21Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/sec21Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/sec21ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/sec21Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/sec21ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
