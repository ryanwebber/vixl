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
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++17")
    # note that std17 is required for a libuv cmake check that tests if libc++ is available
    # TODO: can we make it c++ 20 by default?
endif()

add_subdirectory(3rdParty)
add_subdirectory(Examples)
add_subdirectory(Lib)

message(STATUS "CXX Flags: ${CMAKE_CXX_FLAGS}")
