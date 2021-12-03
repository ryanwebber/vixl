#pragma once

#include <App/RenderLayer.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <memory>
#include <utility>

int CommandInputTextCallback(ImGuiInputTextCallbackData* data);

class GUILayer final : public RenderLayer {
private:
    GLFWwindow* m_Window;
    int OnCommandTextInputEvent(ImGuiInputTextCallbackData* data);

    friend int CommandInputTextCallback(ImGuiInputTextCallbackData*);

public:
    explicit GUILayer(GLFWwindow* window)
        : m_Window(window)
        { }

    void OnInitialize() override;
    void OnDestroy() override;

    void OnRender() override;
    void OnDidRender() override;
};
