include(cmake/resource_utils.cmake)
include(cmake/shader_utils.cmake)

set(TARGET vixl-core)

set(GRAPHICS_API METAL)

set(SOURCES
        src/Application.cpp
        src/Assert.cpp
        src/Closable.cpp
        src/Input.cpp
        src/Logger.cpp
        src/Material.cpp
        src/NativeWindow.cpp
        src/RenderBuffer.cpp
        src/RenderBuiltins.cpp
        src/RenderContext.cpp
        src/RenderPass.cpp
        src/RenderStack.cpp
        src/RenderSystem.cpp
        src/Renderer.cpp
        src/ResourceManager.cpp
        src/Scene.cpp
        src/SceneManager.cpp
        src/SceneRenderer.cpp
        src/Shader.cpp
        src/Texture.cpp
        src/Window.cpp
        src/System/SpriteRenderingSystem.cpp
        )

ADD_LIBRARY(${TARGET} ${SOURCES})

target_compile_features(${TARGET} PUBLIC cxx_std_20)

target_link_libraries(${TARGET}
        PRIVATE
            fmt
            imgui
            pixman
            bimg
        PUBLIC
            stdc++
            glm
            spdlog
            uvw
            EnTT::EnTT
            bx
            bgfx
            SDL2-static
        )

target_include_directories(${TARGET}
        PRIVATE
            "../../3rdParty/fmt/include"
            "../../3rdParty/imgui-wrapper"
            "../../3rdParty/pixman-wrapper/pixman"
            "${PROJECT_SOURCE_DIR}/Lib/Common/include"
            "${PROJECT_SOURCE_DIR}/Lib/Graphics/include"
            "${CMAKE_CURRENT_BINARY_DIR}/include" # Generated code
            "${CMAKE_BINARY_DIR}" # Generated headers for pixman
        PUBLIC
            "../../3rdParty/bgfx/bgfx/include"
            "../../3rdParty/entt/src"
            "../../3rdParty/expected/include"
            "../../3rdParty/glm"
            "../../3rdParty/sdl2/include"
            "../../3rdParty/spdlog/include"
            "../../3rdParty/uvw/src"
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
        )

target_compile_definitions(${TARGET}
        PRIVATE
            VX_DEBUG_MEMORY=1
            VX_DEBUG_GRAPHICS=0
            VX_GRAPHICS_${GRAPHICS_API}=1

            GLM_FORCE_LEFT_HANDED=1 # TODO: This is graphics API dependent
            BGFX_CONFIG_MULTITHREADED=0
        PUBLIC
            VX_VERSION="${CMAKE_PROJECT_VERSION}"
        )

# Shaders

add_compiled_shader(
        TARGET      vixl-shader-sprite_fragment
        TYPE        fragment
        PLATFORM    osx
        PROFILE     metal
        SRC         ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/sprite/fragment.sc
        DEFINITION  ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/sprite/varying.def.sc
        OUTPUT      ${CMAKE_CURRENT_BINARY_DIR}/include/VX/Core/Generated/Shader/Sprite/fragment.h
        NAME        vx_sprite_fragment_shader
        INCLUDES
            ${PROJECT_SOURCE_DIR}/3rdParty/bgfx/bgfx/src
        DEPS
            ${TARGET}
)

add_compiled_shader(
        TARGET      vixl-shader-sprite_vertex
        TYPE        vertex
        PLATFORM    osx
        PROFILE     metal
        SRC         ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/sprite/vertex.sc
        DEFINITION  ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/sprite/varying.def.sc
        OUTPUT      ${CMAKE_CURRENT_BINARY_DIR}/include/VX/Core/Generated/Shader/Sprite/vertex.h
        NAME        vx_sprite_vertex_shader
        INCLUDES
            ${PROJECT_SOURCE_DIR}/3rdParty/bgfx/bgfx/src
        DEPS
            ${TARGET}
)

# Packed assets

add_packed_asset(
        TARGET      vixl-builtins
        MANIFEST    ${CMAKE_CURRENT_SOURCE_DIR}/res/manifest.json
        OUTPUT      ${CMAKE_CURRENT_BINARY_DIR}/Generated/Assets/builtins.asset
        LISTING     ${CMAKE_CURRENT_BINARY_DIR}/include/VX/Core/Generated/Assets/builtins.h
        NAMESPACE   "VX::Core::Generated::Assets::Builtins"
        DEPS
            ${TARGET}
)
