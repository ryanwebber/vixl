#include <App/Dispatcher.h>
#include <App/WorkspaceLayer.h>
#include <App/Size.h>

#include <utility>

WorkspaceLayer::WorkspaceLayer(std::shared_ptr<WorkspaceRegistry> registry, std::shared_ptr<GUIState> gui_state)
    : m_Registry(std::move(registry))
    , m_GUIState(std::move(gui_state))
{
    m_OnWorkspaceOpenedEventHandle = m_Registry->OnWorkspaceOpened([&](auto workspace) {
        Logger::Core->debug("Creating new workspace renderer for {}", workspace->GetIdentifier());
        auto workspace_identifier = workspace->GetIdentifier();
        auto renderer = std::make_unique<WorkspaceRenderer>(workspace);
        SizeInt size = workspace->GetSize();

        auto texture_buffer = renderer->Initialize();
        m_GUIState->OpenWorkspaceWindow(workspace, texture_buffer);

        m_Renderers.insert_or_assign(workspace_identifier, std::move(renderer));
    });

    m_OnWorkspaceClosedEventHandle = m_Registry->OnWorkspaceClosed([&](auto workspace) {
        Logger::Core->debug("Closing workspace renderer for {}", workspace->GetIdentifier());
        auto& renderer = m_Renderers[workspace->GetIdentifier()];

        m_GUIState->CloseWorkspaceWindow(workspace->GetIdentifier());
        renderer->Destroy();

        m_Renderers.erase(workspace->GetIdentifier());
    });
}

void WorkspaceLayer::OnWillRender() {
}

void WorkspaceLayer::OnRender() {
    for (auto &pair : m_Renderers) {
        auto& renderer = pair.second;
        renderer->Render(m_GUIState->GetWorkspaceWindow(pair.first).GetViewport());
    }
}

void WorkspaceLayer::OnDidRender() {
}

