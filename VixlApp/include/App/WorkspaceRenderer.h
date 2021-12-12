#pragma once

#include <memory>
#include <utility>

#include <glm/glm.hpp>
#include <pixman/pixman.h>

#include <Common/Noncopyable.h>

#include <App/Bitmap.h>
#include <App/Colors.h>
#include <App/Workspace.h>

class WorkspaceRenderer {

VX_MAKE_NONCOPYABLE(WorkspaceRenderer);
VX_MAKE_NONMOVABLE(WorkspaceRenderer);

private:
    std::shared_ptr<Workspace> m_Workspace;
    unsigned int m_GLFrameBuffer { };
    unsigned int m_GLTextureBuffer { };
    unsigned int m_GLRenderBuffer { };

public:
    explicit WorkspaceRenderer(std::shared_ptr<Workspace>  workspace)
        : m_Workspace(std::move(workspace))
    {
    };

    ~WorkspaceRenderer() = default;

    UnmanagedBuffer<void> Initialize();

    void Render(const WorkspaceViewport &viewport) const;
    void Destroy();

    std::weak_ptr<Workspace> GetWorkspace() { return m_Workspace; }
};
