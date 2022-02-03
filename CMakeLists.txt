cmake_minimum_required (VERSION 3.6)
project(Vixl VERSION 0.1.0)

# https://cmake.org/cmake/help/latest/policy/CMP0118.html#policy:CMP0118
# This ensures that generated sources propagate the GENERATED source
# property when used out of the originating cmake directory. This means
# that vixl-core can generate sources that are used in example executables,
# and cmake will know to add the right dependencies on the code generator.
# I'm not sure why this is explicitly needed and not the default behavior.
cmake_policy(SET CMP0118 NEW)

# Setup the build for a particular scheme
set(VX_PLATFORM_SCHEME "AUTO" CACHE STRING "The platform scheme to build")
set_property(CACHE VX_PLATFORM_SCHEME PROPERTY STRINGS
        AUTO
        MACOS_DEFAULT
        )

option(VX_BUILD_EXAMPLES "Build the examples" ON)

if (${VX_PLATFORM_SCHEME} STREQUAL "AUTO")
    if(APPLE)
        set(VX_PLATFORM_SCHEME "MACOS_DEFAULT")
    else()
        message(FATAL_ERROR "Unable to determine scheme automatically")
    endif()
endif()

if (${VX_PLATFORM_SCHEME} STREQUAL "MACOS_DEFAULT")
    add_compile_definitions(
            VX_SCHEME_MACOS_DEFAULT=1
    )
else()
    message(FATAL_ERROR "Unknown platform scheme: ${VX_PLATFORM_SCHEME}")
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Preventing an in-tree built.")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The default build type." FORCE)
endif()

if (APPLE)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++20")
endif()

add_subdirectory(3rdParty)
add_subdirectory(Lib)
add_subdirectory(Tools)

if(${VX_BUILD_EXAMPLES})
    add_subdirectory(Examples)
endif()

message(STATUS "CXX Flags: ${CMAKE_CXX_FLAGS}")
