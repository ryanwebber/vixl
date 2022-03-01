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
        PUBLIC
            uvw
            EnTT::EnTT
        )

target_include_directories(${TARGET}
        PRIVATE
            "${PROJECT_SOURCE_DIR}/Lib/Common/include"
            "${PROJECT_SOURCE_DIR}/Lib/Graphics/include"
            "${PROJECT_SOURCE_DIR}/Lib/Platform/include"

            # Generated code
            "${CMAKE_CURRENT_BINARY_DIR}/include"
        PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
        )

target_include_directories(${TARGET}
        SYSTEM
        PRIVATE
            "${PROJECT_SOURCE_DIR}/3rdParty/fmt/include"
        SYSTEM
        PUBLIC
            "${PROJECT_SOURCE_DIR}/3rdParty/entt/src"
            "${PROJECT_SOURCE_DIR}/3rdParty/expected/include"
            "${PROJECT_SOURCE_DIR}/3rdParty/glm"
            "${PROJECT_SOURCE_DIR}/3rdParty/sdl2/include"
            "${PROJECT_SOURCE_DIR}/3rdParty/spdlog/include"
            "${PROJECT_SOURCE_DIR}/3rdParty/uvw/src"
        )

target_compile_definitions(${TARGET}
        PRIVATE
            VX_DEBUG_MEMORY=1
            VX_DEBUG_GRAPHICS=0
            VX_GRAPHICS_${GRAPHICS_API}=1
        PUBLIC
            VX_VERSION="${CMAKE_PROJECT_VERSION}"
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
