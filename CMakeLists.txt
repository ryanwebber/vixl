cmake_minimum_required (VERSION 3.6)
project(Vixl VERSION 0.1.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "The default build type." FORCE)
endif()

add_subdirectory(3rdParty)
add_subdirectory(Examples)
add_subdirectory(Lib)
