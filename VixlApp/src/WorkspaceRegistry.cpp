#include <App/WorkspaceRegistry.h>

std::optional<std::shared_ptr<Workspace>> WorkspaceRegistry::LookupWorkspace(WorkspaceIdentifier id) const {
    auto itr = m_Workspaces.find(id);
    if (itr == m_Workspaces.end())
        return nullptr;

    return itr->second;
}

void WorkspaceRegistry::InsertWorkspace(std::shared_ptr<Workspace> workspace) {
    m_Workspaces.insert_or_assign(workspace->GetIdentifier(), workspace);
}
