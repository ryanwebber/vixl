#pragma once

#include <span>

#include <Common/Noncopyable.h>
#include <App/WorkspaceWindow.h>
#include <App/WorkspaceRegistry.h>

class GUIState {
    VX_MAKE_NONCOPYABLE(GUIState);
    VX_MAKE_NONMOVABLE(GUIState);

private:
    std::vector<WorkspaceWindow> m_WorkspaceWindows;
    std::shared_ptr<WorkspaceRegistry> m_Registry;

public:
    explicit GUIState(std::shared_ptr<WorkspaceRegistry> registry)
        : m_Registry(std::move(registry))
    { };

    ~GUIState() = default;

    void OpenWorkspaceWindow(std::shared_ptr<Workspace>, UnmanagedBuffer<void> render_buffer);
    void CloseWorkspaceWindow(WorkspaceIdentifier identifier);
    [[nodiscard]] const WorkspaceWindow& GetWorkspaceWindow(WorkspaceIdentifier identifier) const;
    WorkspaceWindow& GetWorkspaceWindow(WorkspaceIdentifier identifier);

    [[nodiscard]] std::span<const WorkspaceWindow> GetOpenWorkspaces() const {
        return m_WorkspaceWindows;
    }

    std::span<WorkspaceWindow> GetOpenWorkspaces() {
        return m_WorkspaceWindows;
    }
};
