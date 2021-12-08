#pragma once

#include <App/RenderLayer.h>
#include <App/WorkspaceRegistry.h>
#include <App/WorkspaceRenderer.h>
#include <App/CallbackTask.h>

#include <unordered_map>
#include <memory>

class WorkspaceLayer: public RenderLayer {
private:
    using WorkspaceEventScope = CallbackTask<std::shared_ptr<Workspace>>::Scope;

    std::unordered_map<int, std::unique_ptr<WorkspaceRenderer>> m_Renderers;
    std::shared_ptr<WorkspaceRegistry> m_Registry;

    WorkspaceEventScope m_OnWorkspaceOpenedEventHandle { };
    WorkspaceEventScope m_OnWorkspaceClosedEventHandle { };

    void OnUpdate() override { };

public:
    explicit WorkspaceLayer(std::shared_ptr<WorkspaceRegistry> registry);

    ~WorkspaceLayer() override = default;

    void OnWillRender() override;
    void OnRender() override;
    void OnDidRender() override;
};
