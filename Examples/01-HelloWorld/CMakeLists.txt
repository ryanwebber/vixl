set(TARGET example-hello-world)

GET_PROPERTY(builtins_asset_pack GLOBAL PROPERTY VIXL_BUILTINS_ASSET_PACK)
GET_PROPERTY(builtins_asset_pack_target GLOBAL PROPERTY VIXL_BUILTINS_ASSET_PACK_TARGET)

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
        ${builtins_asset_pack}
        )

set(EXTRA_DEPENDENCIES
        ${builtins_asset_pack_target}
        )

if (APPLE)

    add_executable(${TARGET} MACOSX_BUNDLE ${SOURCES})

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
