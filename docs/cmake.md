# CMake modules

Each module can be found at: ``${PROJECT_ROOT}/cmake `` 

## Signing
Sign any given target using the [Microsoft signtool](https://learn.microsoft.com/en-us/windows/win32/seccrypto/using-signtool-to-sign-a-file).

### Configuration
There are two parameters available for configuring the module

| Parameter | Description | 
|:---|:---|
| WIN_CODE_SIGN_PFX | Name of the Personal Information Exchange (PFX) file |
| WIN_CODE_SIGN_PFX_PASSWORD | Optional password of the PFX file | 


### Example
```cmake
include(sign.cmake)

## ...

add_executable(foo)
# run 'signtool.exe' as a post build step 
sign(foo)

## ...

add_library(bar)
# It could make sense to run signing only in a release build
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    sign(bar)
endif()
```

Only available on Microsoft Windows