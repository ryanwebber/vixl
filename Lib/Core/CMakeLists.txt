
set(TARGET vixl-core)

set(INCLUDES
        )

set(SOURCES
        src/Application.cpp
        src/Assert.cpp
        src/Bitmap.cpp
        src/Logger.cpp
        src/Material.cpp
        src/NativeWindow.cpp
        src/RenderStack.cpp
        src/TimerLoopTask.cpp
        src/TimerTask.cpp
        src/Window.cpp
        src/Renderer.cpp src/Input.cpp src/SceneRenderer.cpp src/Scene.cpp src/RenderCamera.cpp src/RenderService.cpp src/RenderSystem.cpp src/Closable.cpp)

ADD_LIBRARY(${TARGET} ${SOURCES})

target_compile_features(${TARGET} PUBLIC cxx_std_20)

target_link_libraries(${TARGET}
        PRIVATE
            c++
            fmt
            glfw
            imgui
            pixman
            bx
            bimg
            bgfx
        PUBLIC
            glm
            spdlog
            uvw
        )

target_include_directories(${TARGET}
        PRIVATE
            "../../3rdParty/fmt/include"
            "../../3rdParty/glfw/include"
            "../../3rdParty/imgui-wrapper"
            "../../3rdParty/pixman-wrapper/pixman"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Common/include"
            "${CMAKE_BINARY_DIR}" # Generated headers for pixman
        PUBLIC
            "../../3rdParty/glm"
            "../../3rdParty/spdlog/include"
            "../../3rdParty/expected/include"
            "../../3rdParty/uvw/src"
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
        )

target_compile_definitions(${TARGET}
        PRIVATE
            DEBUG_MEMORY=1
        PUBLIC
            VX_VERSION="${CMAKE_PROJECT_VERSION}"
        )

set(CMAKE_CXX_STANDARD_REQUIRED ON)
