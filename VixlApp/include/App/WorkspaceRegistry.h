#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

#include <Common/Noncopyable.h>

#include <App/CallbackTask.h>
#include <App/EventLoop.h>
#include <App/Workspace.h>

class WorkspaceRegistry {
    VX_MAKE_NONMOVABLE(WorkspaceRegistry);
    VX_MAKE_NONCOPYABLE(WorkspaceRegistry);

private:
    std::unordered_map<WorkspaceIdentifier, std::shared_ptr<Workspace>> m_Workspaces;

    std::shared_ptr<CallbackTask<std::shared_ptr<Workspace>>> m_WorkspaceOpenedCallback;
    std::shared_ptr<CallbackTask<std::shared_ptr<Workspace>>> m_WorkspaceClosedCallback;

public:
    explicit WorkspaceRegistry(EventLoop& event_loop);
    ~WorkspaceRegistry() = default;

    [[nodiscard]] std::optional<std::shared_ptr<Workspace>> LookupWorkspace(WorkspaceIdentifier id) const;
    void InsertWorkspace(std::shared_ptr<Workspace> workspace);
    void CloseWorkspace(WorkspaceIdentifier id);

    void ForEachWorkspace(const std::function<void(Workspace&)>& callback) const {
        for (auto&& entry : m_Workspaces) {
            callback(*entry.second);
        }
    }

    [[nodiscard]] CallbackTask<std::shared_ptr<Workspace>>::Scope OnWorkspaceOpened(CallbackTask<std::shared_ptr<Workspace>>::Callback &&callback) const;
    [[nodiscard]] CallbackTask<std::shared_ptr<Workspace>>::Scope OnWorkspaceClosed(CallbackTask<std::shared_ptr<Workspace>>::Callback &&callback) const;
};
