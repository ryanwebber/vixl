include(cmake/resource_utils.cmake)
include(cmake/shader_utils.cmake)

set(TARGET vixl-core)

set(GRAPHICS_API METAL)

set(SHADERS
        res/shaders/colored
        res/shaders/uvmap
        )

set(SOURCES
        src/Application.cpp
        src/Assert.cpp
        src/Bitmap.cpp
        src/Closable.cpp
        src/DemoRenderSystem.cpp
        src/FileReadTask.cpp
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
        src/TimerLoopTask.cpp
        src/TimerTask.cpp
        src/Window.cpp
        )

ADD_LIBRARY(${TARGET} ${SOURCES})

add_compiled_shader_dependency(${TARGET} "${SHADERS}")
add_builtins_asset_pack_dependency(${TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/res/manifest.json ${CMAKE_CURRENT_SOURCE_DIR})

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
            glfw
            spdlog
            uvw
            EnTT::EnTT
            bx
            bgfx
        )

target_include_directories(${TARGET}
        PRIVATE
            "../../3rdParty/fmt/include"
            "../../3rdParty/glfw/include"
            "../../3rdParty/imgui-wrapper"
            "../../3rdParty/pixman-wrapper/pixman"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Common/include"
            "${CMAKE_CURRENT_BINARY_DIR}/include" # Generated code
            "${CMAKE_BINARY_DIR}" # Generated headers for pixman
        PUBLIC
            "../../3rdParty/glm"
            "../../3rdParty/spdlog/include"
            "../../3rdParty/expected/include"
            "../../3rdParty/entt/src"
            "../../3rdParty/uvw/src"
            "../../3rdParty/bgfx/bgfx/include"
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
        )

target_compile_definitions(${TARGET}
        PRIVATE
            VX_DEBUG_MEMORY=1
            VX_GRAPHICS_${GRAPHICS_API}=1
            GLM_FORCE_LEFT_HANDED=1 # TODO: This is graphics API dependent
        PUBLIC
            VX_VERSION="${CMAKE_PROJECT_VERSION}"
        )
