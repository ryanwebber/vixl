set(TARGET vixl-graphics)

option(VX_GRAPHICS_MACOS_SUPPORT "Include macOS support" OFF)

# Grab the Vulkan SDK for the development platform: https://vulkan.lunarg.com
# More info on the FindVulkan cmake module: https://cmake.org/cmake/help/latest/module/FindVulkan.html
find_package(Vulkan REQUIRED FATAL_ERROR)

message(STATUS "Vulkan info:\n\tLibs: ${Vulkan_LIBRARIES}\n\tincludes: ${Vulkan_INCLUDE_DIRS}\n")

set(SOURCES
        src/Init.cpp
        src/Instance.cpp
        src/QueueSupport.cpp
        src/RenderPass.cpp
        src/RenderPipeline.cpp
        src/Swapchain.cpp
        )

set(DEFINITIONS
        )

if(${VX_GRAPHICS_MACOS_SUPPORT})
    list(APPEND SOURCES
            src/Init_MacOS.cpp
            )

    list(APPEND DEFINITIONS
            VK_USE_PLATFORM_METAL_EXT=1
            )
endif()

ADD_LIBRARY(${TARGET} STATIC ${SOURCES})

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET}
        PUBLIC
            ${Vulkan_LIBRARIES}
        )

target_include_directories(${TARGET}
        SYSTEM
        PRIVATE
            ${Vulkan_INCLUDE_DIRS}
            ${PROJECT_SOURCE_DIR}/3rdParty/spdlog/include
        SYSTEM
        PUBLIC
            ${PROJECT_SOURCE_DIR}/3rdParty/glm
        )

target_include_directories(${TARGET}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/Lib/Common/include
        )

target_compile_definitions(${TARGET} PRIVATE ${DEFINITIONS})
target_compile_definitions(${TARGET} PRIVATE
        VERSION="${CMAKE_PROJECT_VERSION}"
        VX_GRAPHICS_DEBUG=1
        VX_GRAPHICS_VALIDATION=1
        )

set(CMAKE_CXX_STANDARD_REQUIRED ON)
