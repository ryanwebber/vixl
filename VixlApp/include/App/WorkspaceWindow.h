#pragma once

#include <string>

#include <Common/Noncopyable.h>
#include <Common/Copyable.h>

#include <App/Workspace.h>
#include <App/WorkspaceViewport.h>
#include <App/UnmanagedBuffer.h>

class WorkspaceWindow {
    VX_MAKE_NONCOPYABLE(WorkspaceWindow);
    VX_DEFAULT_MOVABLE(WorkspaceWindow);

private:
    std::string m_Identifier;
    WorkspaceViewport m_Viewport;
    UnmanagedBuffer<void> m_FrameBuffer;
    std::shared_ptr<Workspace> m_Workspace;
    void* m_WindowHandle;

public:
    WorkspaceWindow(std::shared_ptr<Workspace> workspace, UnmanagedBuffer<void> frame_buffer)
        : m_Viewport(workspace->GetSize().cast<float>())
        , m_Identifier(std::move(fmt::format("Workspace##{}", workspace->GetIdentifier())))
        , m_FrameBuffer(std::move(frame_buffer))
        , m_Workspace(std::move(workspace))
        , m_WindowHandle(nullptr)
    {};

    ~WorkspaceWindow() = default;

    [[nodiscard]] const WorkspaceViewport& GetViewport() const { return m_Viewport; }
    [[nodiscard]] UnmanagedBuffer<void> GetFrameBuffer() const { return m_FrameBuffer; }
    [[nodiscard]] const Workspace& GetWorkspace() const { return *m_Workspace; }

    [[nodiscard]] void* GetWindowHandle() const { return m_WindowHandle; }
    void SetWindowHandle(void* handle) { m_WindowHandle = handle; }

    const std::string& GetWindowIdentifier() const { return m_Identifier; }
};
