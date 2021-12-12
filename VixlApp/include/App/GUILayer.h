#pragma once

#include <memory>
#include <utility>

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <App/RenderLayer.h>
#include <App/WorkspaceRegistry.h>
#include <App/GUIState.h>

int CommandInputTextCallback(ImGuiInputTextCallbackData* data);

class GUILayer final : public RenderLayer {
private:
    GLFWwindow* m_Window;
    std::shared_ptr<GUIState> m_GUIState;

    int OnCommandTextInputEvent(ImGuiInputTextCallbackData* data);
    friend int CommandInputTextCallback(ImGuiInputTextCallbackData*);

public:
    explicit GUILayer(GLFWwindow* window, std::shared_ptr<GUIState> gui_state)
        : m_Window(window)
        , m_GUIState(std::move(gui_state))
        { }

    void OnInitialize() override;
    void OnDestroy() override;

    void OnRender() override;
    void OnDidRender() override;
};
