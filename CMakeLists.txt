cmake_minimum_required (VERSION 3.6)
project(Vixl VERSION 0.1.0)

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

# Set public variables to be consumed by API users
DEFINE_PROPERTY(GLOBAL PROPERTY VIXL_BUILTINS_ASSET_PACK
        BRIEF_DOCS "The builtins asset pack to be installed somewhere on the target system."
        FULL_DOCS  "The builtins asset pack to be installed somewhere on the target system.")
DEFINE_PROPERTY(GLOBAL PROPERTY VIXL_BUILTINS_ASSET_PACK_TARGET
        BRIEF_DOCS "Target name for generating the asset pack."
        FULL_DOCS  "Target name for generating the asset pack. Add a dependency on this.")

add_subdirectory(3rdParty)
add_subdirectory(Tools)
add_subdirectory(Lib)
add_subdirectory(Examples)

message(STATUS "CXX Flags: ${CMAKE_CXX_FLAGS}")
