#pragma once

#include <memory>
#include <unordered_map>

#include <App/Workspace.h>
#include <Common/Noncopyable.h>

class WorkspaceRegistry {
    VX_MAKE_NONMOVABLE(WorkspaceRegistry);
    VX_MAKE_NONCOPYABLE(WorkspaceRegistry);

private:
    std::unordered_map<WorkspaceIdentifier, std::shared_ptr<Workspace>> m_Workspaces;

public:
    WorkspaceRegistry() = default;
    ~WorkspaceRegistry() = default;

    [[nodiscard]] std::optional<std::shared_ptr<Workspace>> LookupWorkspace(WorkspaceIdentifier id) const;
    void InsertWorkspace(std::shared_ptr<Workspace> workspace);
};
