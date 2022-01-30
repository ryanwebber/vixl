set(TARGET vixl-graphics)

# Grab the Vulkan SDK for the development platform: https://vulkan.lunarg.com
# More info on the FindVulkan cmake module: https://cmake.org/cmake/help/latest/module/FindVulkan.html
find_package(Vulkan REQUIRED FATAL_ERROR)

set(SOURCES
        src/Graphics.cpp
        )

ADD_LIBRARY(${TARGET} STATIC ${SOURCES})

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET}
        PRIVATE
            ${Vulkan_LIBRARIES}
        )

target_include_directories(${TARGET}
        PRIVATE
            ${Vulkan_INCLUDE_DIRS}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/3rdParty/glm
        )

target_compile_definitions(${TARGET} PRIVATE VERSION="${CMAKE_PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
