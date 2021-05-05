
## set(CMAKE_CXX_STANDARD 20)
# output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


find_program(CCACHE ccache)
if(CCACHE)
  message(STATUS "using ccache")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# Generate compile_commands.json to make it easier to work with clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
## TODO: possible to connect with: https://www.sourcetrail.com/


option(ENABLE_IPO "Enable Iterprocedural Optimization, aka Link Time Optimization (LTO)" OFF)
if(ENABLE_IPO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT result OUTPUT output)
  if(result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(SEND_ERROR "IPO is not supported: ${output}")
  endif()
endif()
