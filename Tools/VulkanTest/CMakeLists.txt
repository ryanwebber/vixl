include("${PROJECT_SOURCE_DIR}/Lib/Entry/cmake/vixl_application.cmake")

set(TARGET vulkan-test)

set(SOURCES
        main.cpp
        )

add_vixl_application(
        TARGET ${TARGET}
        PLATFORM macOS
        VULKAN_SDK $ENV{VULKAN_SDK}
        SOURCES ${SOURCES}
        )
