#pragma once

#include <App/RenderLayer.h>
#include <App/WorkspaceRegistry.h>
#include <App/WorkspaceRenderer.h>

#include <unordered_map>
#include <memory>

class WorkspaceLayer: public RenderLayer {
private:
    std::shared_ptr<WorkspaceRegistry> m_Registry;
    std::unordered_map<int, std::unique_ptr<WorkspaceRenderer>> m_Renderers;

    void OnUpdate() override { };

public:
    explicit WorkspaceLayer(std::shared_ptr<WorkspaceRegistry> registry)
        : m_Registry(std::move(registry))
    {
    };

    ~WorkspaceLayer() override = default;

    void OnWillRender() override { };
    void OnRender() override { };
    void OnDidRender() override { };
};
