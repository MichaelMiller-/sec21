# sec21

[![GCC](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc.yml) [![Clang](https://github.com/MichaelMiller-/sec21/actions/workflows/clang.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/clang.yml) [![MSVC](https://github.com/MichaelMiller-/sec21/actions/workflows/msvc.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/msvc.yml) [![AppVeyor MSVC2019](https://ci.appveyor.com/api/projects/status/4s6bg4yexj0cna45?svg=true)](https://ci.appveyor.com/project/MichaelMiller-/sec21)

[![codecov](https://codecov.io/gh/MichaelMiller-/sec21/branch/master/graph/badge.svg?token=f7vlTsHZWl)](https://codecov.io/gh/MichaelMiller-/sec21) [![CodeQL](https://github.com/MichaelMiller-/sec21/actions/workflows/codeql.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/codeql.yml)

**sec21** is a collection of useful functions and classes and also the boilerplate for a lot of my projects.

## Documentation
See the [Wiki](https://github.com/MichaelMiller-/sec21/wiki) for the full documentation, examples, operational details and other information.


## Usage

### Build **sec21** from Sources
Building with CMake is pretty easy, just invoke the following shell commands:
```sh
git clone https://github.com/MichaelMiller-/sec21
cd sec21
mkdir _build && cd _build/
cmake ..
sudo make install
```

### Usage in CMake projects
If you have installed **sec21** system-wide you can use the CMake-package-system.
```cmake
find_package(sec21 CONFIG REQUIRED)
## ...
target_link_libraries(target_name PRIVATE sec21::sec21)
```


## Get
There a are several methods to get the library.

### Use CPM
One of them is the setup-free CMake [CPM](https://github.com/cpm-cmake/CPM.cmake) dependency manager:
```cmake
CPMAddPackage("gh:MichaelMiller-/sec21@1.0.1")
## ...
target_link_libraries(target_name PRIVATE sec21::sec21)
```

### Use Vcpkg
You also can download and install **sec21** using the [vcpkg](https://github.com/Microsoft/vcpkg) dependency manager:
```sh
$ vcpkg install michaelmiller-sec21
```

### Using FetchContent
Alternatively, the project can also be used only in a private context. Simply add the following to your CMakeLists.txt:
```cmake
include(FetchContent)
FetchContent_Declare(sec21
        GIT_REPOSITORY https://github.com/MichaelMiller-/sec21
        # GIT_TAG ... # optional tag
        )
FetchContent_MakeAvailable(sec21)
## If BUILD_TESTING for sec21 is enabled make sure that Catch2 is available.
## find_package(Catch2 CONFIG REQUIRED)
set_target_properties(sec21 PROPERTIES BUILD_TESTING FALSE)

## Link against sec21::sec21
target_link_libraries(main PRIVATE sec21::sec21)
```

## License
Please see LICENSE.