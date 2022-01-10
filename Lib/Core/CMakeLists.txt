include(cmake/shader_utils.cmake)

set(TARGET vixl-core)

set(SHADERS
        res/shaders/uvtex
        )

set(SOURCES
        src/Application.cpp
        src/Assert.cpp
        src/Bitmap.cpp
        src/Closable.cpp
        src/DemoRenderSystem.cpp
        src/Input.cpp
        src/Logger.cpp
        src/Material.cpp
        src/NativeWindow.cpp
        src/RenderStack.cpp
        src/RenderSystem.cpp
        src/RenderTarget.cpp
        src/Renderer.cpp
        src/Scene.cpp
        src/SceneManager.cpp
        src/SceneRenderer.cpp
        src/TimerLoopTask.cpp
        src/TimerTask.cpp
        src/Window.cpp
        src/RenderPass.cpp src/Shader.cpp)

ADD_LIBRARY(${TARGET} ${SOURCES})

foreach (shader IN LISTS SHADERS)
    add_compiled_shader_dependency(${TARGET} fragment ${shader})
    add_compiled_shader_dependency(${TARGET} vertex ${shader})
endforeach()

target_compile_features(${TARGET} PUBLIC cxx_std_20)

target_link_libraries(${TARGET}
        PRIVATE
            fmt
            glfw
            imgui
            pixman
            bx
            bimg
            bgfx
        PUBLIC
            stdc++
            glm
            spdlog
            uvw
            EnTT::EnTT
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
            DEBUG_MEMORY=1
        PUBLIC
            VX_VERSION="${CMAKE_PROJECT_VERSION}"
        )

