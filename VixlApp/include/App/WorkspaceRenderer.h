#pragma once

#include <Common/Noncopyable.h>

class WorkspaceRenderer {

VX_MAKE_NONCOPYABLE(WorkspaceRenderer);
VX_MAKE_NONMOVABLE(WorkspaceRenderer);

private:
    std::weak_ptr<Workspace> m_Workspace;

public:
    explicit WorkspaceRenderer(const std::shared_ptr<Workspace>& workspace)
        : m_Workspace(workspace)
        { };

    ~WorkspaceRenderer() = default;

    void OnWillRender() { }
    void OnRender() { };
    void OnDidRender() { };

    std::weak_ptr<Workspace> GetWorkspace() { return m_Workspace; }
};
