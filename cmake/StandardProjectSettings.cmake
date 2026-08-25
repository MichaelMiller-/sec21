
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message(STATUS "using ccache")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# Generate compile_commands.json to make it easier to work with clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
## TODO: possible to connect with: https://www.sourcetrail.com/


if(CMAKE_VERSION VERSION_GREATER 3.8)
  # Enable IPO for CMake versions that support it
  cmake_policy(SET CMP0069 NEW)
else()

endif()

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
