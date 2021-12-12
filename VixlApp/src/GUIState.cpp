#include <App/GUIState.h>

#include <utility>

void GUIState::OpenWorkspaceWindow(std::shared_ptr<Workspace> workspace, UnmanagedBuffer<void> frame_buffer) {
    WorkspaceWindow window(std::move(workspace), std::move(frame_buffer));
    m_WorkspaceWindows.push_back(std::move(window));
}

void GUIState::CloseWorkspaceWindow(WorkspaceIdentifier identifier) {
    m_WorkspaceWindows.erase(std::remove_if(m_WorkspaceWindows.begin(), m_WorkspaceWindows.end(), [&](const WorkspaceWindow &elem) {
        return elem.GetWorkspace().GetIdentifier() == identifier;
    }), m_WorkspaceWindows.end());

    m_Registry->CloseWorkspace(identifier);
}

const WorkspaceWindow& GUIState::GetWorkspaceWindow(WorkspaceIdentifier identifier) const {
    auto it = std::find_if(m_WorkspaceWindows.begin(), m_WorkspaceWindows.end(), [&](const WorkspaceWindow &elem) {
        return elem.GetWorkspace().GetIdentifier() == identifier;
    });

    return *it;
}

WorkspaceWindow& GUIState::GetWorkspaceWindow(WorkspaceIdentifier identifier) {
    auto it = std::find_if(m_WorkspaceWindows.begin(), m_WorkspaceWindows.end(), [&](const WorkspaceWindow &elem) {
        return elem.GetWorkspace().GetIdentifier() == identifier;
    });

    return *it;
}
