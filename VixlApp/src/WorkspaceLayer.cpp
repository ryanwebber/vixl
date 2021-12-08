#include <App/Dispatcher.h>
#include <App/WorkspaceLayer.h>

#include <utility>

WorkspaceLayer::WorkspaceLayer(std::shared_ptr<WorkspaceRegistry> registry)
    : m_Registry(std::move(registry))
{
    m_OnWorkspaceOpenedEventHandle = m_Registry->OnWorkspaceOpened([&](auto workspace) {
        auto workspace_identifier = workspace->GetIdentifier();
        auto renderer = std::make_unique<WorkspaceRenderer>(std::move(workspace));
        renderer->Initialize();

        m_Renderers.insert_or_assign(workspace_identifier, std::move(renderer));
    });

    m_OnWorkspaceClosedEventHandle = m_Registry->OnWorkspaceClosed([&](auto workspace) {
        auto& renderer = m_Renderers[workspace->GetIdentifier()];
        renderer->Destroy();

        m_Renderers.erase(workspace->GetIdentifier());
    });
}

void WorkspaceLayer::OnWillRender() {
}

void WorkspaceLayer::OnRender() {
    for (auto &pair : m_Renderers) {
        auto& renderer = pair.second;
        renderer->Render();
    }
}

void WorkspaceLayer::OnDidRender() {
}

