# Welcome & getting started

**sec21** is a collection of useful functions and classes and the boilerplate for a lot of my projects.

## Build and install using CMake
Building with CMake on Linux is very easy, just use the following shell commands:
```sh
git clone https://github.com/MichaelMiller-/sec21
cd sec21
mkdir _build && cd _build/
cmake ..
sudo make install
```

## Usage in CMake projects
If you have installed **sec21** system-wide you can use the CMake-package-system.
```cmake
find_package(sec21 CONFIG REQUIRED)
## ...
target_link_libraries(${PROJECT_NAME} PRIVATE sec21::sec21)
```

## Using FetchContent

Alternatively, the project can also be used only in a private context. Simply add the following to your CMakeLists.txt:
```cmake
## If BUILD_TESTING for sec21 is enabled make sure that Catch2 is available.
## find_package(Catch2 CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(sec21
        GIT_REPOSITORY https://github.com/MichaelMiller-/sec21
        GIT_TAG ... # Commit hash
        )
FetchContent_MakeAvailable(sec21)
set_target_properties(sec21 PROPERTIES BUILD_TESTING FALSE)

## Link against sec21::sec21
target_link_libraries(${PROJECT_NAME} PRIVATE sec21::sec21)
```
