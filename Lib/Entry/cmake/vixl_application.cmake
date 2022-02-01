set(VIXL_APPLICATION_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR})

function(add_vixl_application)
    cmake_parse_arguments(
            ARG
            ""
            "TARGET;PLATFORM;VULKAN_SDK"
            "SOURCES"
            ${ARGN}
    )

    if(NOT ARG_VULKAN_SDK)
        message(FATAL_ERROR "Vulkan SDK path required via argument VULKAN_SDK.")
    endif()

    set(APPLICATION_SOURCES
            ${VIXL_APPLICATION_CMAKE_DIR}/../src/Inlines.cpp
            )

    list(APPEND APPLICATION_SOURCES ${ARG_SOURCES})

    if(${ARG_PLATFORM} STREQUAL macOS)

        # Application bundle layout should look like:
        # ---
        # [application name].app
        #     Contents
        #         Frameworks
        #             libMoltenVK.dylib
        #             libvulkan.1.[version number].dylib
        #             libvulkan.1.dylib -> libvulkan.1.[version number].dylib
        #         MacOS
        #             [application name]
        #         Resources
        #             vulkan
        #                 icd.d
        #                     MoltenVK_icd.json

        set(APP_BUNDLE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${ARG_TARGET}.app)

        set(MOLTEN_VK_SDK ${ARG_VULKAN_SDK}/../MoltenVK)
        set(MOLTEN_VK_ICD_FILE ${VIXL_APPLICATION_CMAKE_DIR}/../res/platform/macOS/MoltenVK_icd.json)

        list(APPEND APPLICATION_SOURCES ${MOLTEN_VK_ICD_FILE})

        add_executable(${ARG_TARGET} MACOSX_BUNDLE ${APPLICATION_SOURCES})

        target_include_directories(${ARG_TARGET} PRIVATE
                ${PROJECT_SOURCE_DIR}/Lib/Graphics/include
                ${PROJECT_SOURCE_DIR}/Lib/Core/include
                ${PROJECT_SOURCE_DIR}/Lib/Common/include
                ${PROJECT_SOURCE_DIR}/Lib/Entry/include
                )

        set(APPLICATION_LIBS
                vixl-core
                vixl-common
                vixl-entry
                vixl-graphics
                )

        find_library(COCOA_LIBRARY Cocoa)
        find_library(QUARTZ_LIBRARY QuartzCore)
        list(APPEND APPLICATION_LIBS ${COCOA_LIBRARY} ${QUARTZ_LIBRARY})

        target_link_libraries(${ARG_TARGET} ${APPLICATION_LIBS})

        set_property(SOURCE ${MOLTEN_VK_ICD_FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/vulkan/icd.d")

        file(GLOB VULKAN_DYLIB ${ARG_VULKAN_SDK}/lib/libvulkan.1.*.dylib)
        set(VULKAN_DYLIBS
                ${MOLTEN_VK_SDK}/dylib/macOS/libMoltenVK.dylib
                ${VULKAN_DYLIB}
                )

        foreach(DYLIB ${VULKAN_DYLIBS})
            get_filename_component(DYLIB_NAME ${DYLIB} NAME)
            add_custom_command(
                    TARGET ${ARG_TARGET}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_BUNDLE_DIR}/Contents/Frameworks"
                    COMMAND ${CMAKE_COMMAND} -E copy ${DYLIB} "${APP_BUNDLE_DIR}/Contents/Frameworks/"
                    DEPENDS vulkan)
        endforeach()

        get_filename_component(VULKAN_DYLIB_NAME ${VULKAN_DYLIB} NAME)
        add_custom_command(
                TARGET ${ARG_TARGET}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E create_symlink ${VULKAN_DYLIB_NAME} "${APP_BUNDLE_DIR}/Contents/Frameworks/libvulkan.1.dylib"
                DEPENDS vulkan)

        # Fixup the dylib path for vulkan SDK to look for the dylib we're shipping in Contents/Frameworks
        add_custom_command(
                TARGET ${ARG_TARGET}
                POST_BUILD
                COMMAND install_name_tool -change "@rpath/libvulkan.1.dylib" "@executable_path/../Frameworks/libvulkan.1.dylib" "${APP_BUNDLE_DIR}/Contents/MacOS/${ARG_TARGET}"
                DEPENDS vulkan
                )

        target_compile_definitions(${ARG_TARGET}
                PRIVATE
                    VX_PLATFORM_MACOS=1
                )
    else()
        message(FATAL_ERROR "Unsupported platform: ${ARG_PLATFORM}")
    endif()
endfunction()