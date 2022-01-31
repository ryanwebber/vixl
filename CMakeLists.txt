cmake_minimum_required (VERSION 3.6)
project(Vixl VERSION 0.1.0)

# https://cmake.org/cmake/help/latest/policy/CMP0118.html#policy:CMP0118
# This ensures that generated sources propagate the GENERATED source
# property when used out of the originating cmake directory. This means
# that vixl-core can generate sources that are used in example executables,
# and cmake will know to add the right dependencies on the code generator.
# I'm not sure why this is explicitly needed and not the default behavior.
cmake_policy(SET CMP0118 NEW)

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
add_subdirectory(Examples)

message(STATUS "CXX Flags: ${CMAKE_CXX_FLAGS}")
