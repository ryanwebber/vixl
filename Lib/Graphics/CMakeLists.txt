set(TARGET vixl-graphics)

# Grab the Vulkan SDK for the development platform: https://vulkan.lunarg.com
# More info on the FindVulkan cmake module: https://cmake.org/cmake/help/latest/module/FindVulkan.html
find_package(Vulkan REQUIRED FATAL_ERROR)

message(STATUS "Vulkan info:\n\tLibs: ${Vulkan_LIBRARIES}\n\tincludes: ${Vulkan_INCLUDE_DIRS}\n")

set(SOURCES
        src/Instance.cpp
        )

ADD_LIBRARY(${TARGET} STATIC ${SOURCES})

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET}
        PUBLIC
            ${Vulkan_LIBRARIES}
        )

target_include_directories(${TARGET} SYSTEM PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(${TARGET}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/3rdParty/spdlog/include
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/3rdParty/glm
            ${PROJECT_SOURCE_DIR}/Lib/Common/include
        )

target_compile_definitions(${TARGET} PRIVATE
        VERSION="${CMAKE_PROJECT_VERSION}"
        VX_GRAPHICS_DEBUG=1
        VX_GRAPHICS_VALIDATION=1
        )

set(CMAKE_CXX_STANDARD_REQUIRED ON)
