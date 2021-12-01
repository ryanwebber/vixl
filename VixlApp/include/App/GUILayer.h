#pragma once

#include <App/RenderLayer.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <utility>

class GUILayer final : public RenderLayer {
private:
    GLFWwindow* m_Window;
    bool m_ShowDemoWindow;

public:
    explicit GUILayer(GLFWwindow* window)
        : m_Window(window)
        , m_ShowDemoWindow(true)
        { }

    void OnInitialize() override;
    void OnDestroy() override;

    void OnRender() override;
    void OnDidRender() override;
};
