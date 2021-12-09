#include <App/WorkspaceRegistry.h>

#include <utility>

WorkspaceRegistry::WorkspaceRegistry(EventLoop &event_loop) {
    m_WorkspaceOpenedCallback = event_loop.Register<CallbackTask<std::shared_ptr<Workspace>>>("OnWorkspaceOpened");
    m_WorkspaceClosedCallback = event_loop.Register<CallbackTask<std::shared_ptr<Workspace>>>("OnWorkspaceClosed");
}

std::optional<std::shared_ptr<Workspace>> WorkspaceRegistry::LookupWorkspace(WorkspaceIdentifier id) const {
    auto itr = m_Workspaces.find(id);
    if (itr == m_Workspaces.end())
        return nullptr;

    return itr->second;
}

void WorkspaceRegistry::InsertWorkspace(std::shared_ptr<Workspace> workspace) {
    m_Workspaces.insert_or_assign(workspace->GetIdentifier(), workspace);
    m_WorkspaceOpenedCallback->Notify(workspace);
}

void WorkspaceRegistry::CloseWorkspace(WorkspaceIdentifier id) {
    auto itr = m_Workspaces.find(id);
    if (itr != m_Workspaces.end()) {
        m_Workspaces.erase(itr);
        m_WorkspaceClosedCallback->Notify(itr->second);
    }
}


CallbackTask<std::shared_ptr<Workspace>>::Scope
WorkspaceRegistry::OnWorkspaceOpened(CallbackTask<std::shared_ptr<Workspace>>::Callback&& callback) const {
    return std::move(m_WorkspaceOpenedCallback->OnCallback(callback));
}

CallbackTask<std::shared_ptr<Workspace>>::Scope
WorkspaceRegistry::OnWorkspaceClosed(CallbackTask<std::shared_ptr<Workspace>>::Callback&& callback) const {
    return std::move(m_WorkspaceClosedCallback->OnCallback(callback));
}
