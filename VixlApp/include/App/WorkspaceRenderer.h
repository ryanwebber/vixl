#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <utility>
#include <Common/Noncopyable.h>

#include <App/Workspace.h>

class WorkspaceRenderer {

VX_MAKE_NONCOPYABLE(WorkspaceRenderer);
VX_MAKE_NONMOVABLE(WorkspaceRenderer);

private:
    std::shared_ptr<Workspace> m_Workspace;

    glm::vec2 m_ViewportSize;

    unsigned int m_GLFrameBuffer { };
    unsigned int m_GLTextureBuffer { };
    unsigned int m_GLRenderBuffer { };

public:
    explicit WorkspaceRenderer(std::shared_ptr<Workspace>  workspace)
        : m_Workspace(std::move(workspace))
        , m_ViewportSize({ 800, 600 })
    {
    };

    ~WorkspaceRenderer() = default;

    void Initialize();
    void Render() const;
    void Destroy();

    std::weak_ptr<Workspace> GetWorkspace() { return m_Workspace; }
};
