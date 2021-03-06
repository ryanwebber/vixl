set(TARGET example-hello-world)

set(BUILTINS_TARGET vixl-builtins)

get_target_property(BUILTINS_ASSET_BUNDLE ${BUILTINS_TARGET} ASSET_BUNDLE)

set(LIBS
        vixl-common
        vixl-core
        )

set(INCLUDES
        "${CMAKE_CURRENT_SOURCE_DIR}/../Lib/Common/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Lib/Core/include"
        )

set(SOURCES
        main.cpp
        )

set(RESOURCES
        ${BUILTINS_ASSET_BUNDLE}
        )

set(EXTRA_DEPENDENCIES
        ${BUILTINS_TARGET}
        )

if (APPLE)

    add_executable(${TARGET} MACOSX_BUNDLE ${SOURCES} ${RESOURCES})

    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    list(APPEND LIBS ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY})

    set_target_properties(${TARGET} PROPERTIES
            OUTPUT_NAME "${TARGET}"
            MACOSX_BUNDLE TRUE
            MACOSX_RPATH TRUE
            MACOSX_FRAMEWORK_IDENTIFIER com.example.application
            XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@loader_path/Libraries"
            RESOURCE "${RESOURCES}"
            XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME TRUE
            XCODE_ATTRIBUTE_EXECUTABLE_NAME "${TARGET}"
            )

    # Copy resources into macOS app resource bundle
    foreach(RESOURCE ${RESOURCES})
        get_filename_component(basename ${RESOURCE} NAME)
        set_property(SOURCE ${RESOURCE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${basename}")
    endforeach()
else()
    message(FATAL "Unsupported platform")
endif()

target_compile_features(${TARGET} PRIVATE cxx_std_20)

target_include_directories(${TARGET} PRIVATE ${INCLUDES})
target_link_libraries(${TARGET} ${LIBS})

add_dependencies(${TARGET} ${EXTRA_DEPENDENCIES})