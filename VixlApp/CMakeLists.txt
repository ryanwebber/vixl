project(vixl CXX)

set(TARGET vixl)

set(LIBS
    imgui
    glfw
    spdlog
    )

set(INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "../3rdParty/GLFW/include"
    "../3rdParty/spdlog/include"
    "../3rdParty/imgui-wrapper"
    )

set(SOURCES
    src/main.cpp
    src/Logger.cpp
    src/RenderStack.cpp
    src/GUILayer.cpp
    )

set(RESOURCES
    )

add_executable(${TARGET} ${SOURCES})

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    list(APPEND LIBS ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY})

    set_target_properties(${TARGET} PROPERTIES
        OUTPUT_NAME "Vixl"
        MACOSX_BUNDLE TRUE
        MACOSX_RPATH TRUE
        MACOSX_FRAMEWORK_IDENTIFIER ca.ryanwebber.vixl
        XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@loader_path/Libraries"
        RESOURCE "${RESOURCES}"
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME TRUE
        XCODE_ATTRIBUTE_EXECUTABLE_NAME "${TARGET}"
    )
else()
    message(FATAL "Unsupported platform")
endif()

if(PLATFORM_WIN32 OR PLATFORM_LINUX)
    # Copy assets to target folder
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "\"$<TARGET_FILE_DIR:${TARGET}>\"")
endif()

target_compile_features(${TARGET} PRIVATE cxx_std_17)
target_include_directories(${TARGET} PUBLIC ${INCLUDES})
target_link_libraries(${TARGET} ${LIBS})
target_compile_definitions(${TARGET} PRIVATE VERSION="${CMAKE_PROJECT_VERSION}")
