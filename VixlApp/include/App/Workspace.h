#pragma once

#include <glm/glm.hpp>

#include <App/Logger.h>
#include <App/WorkspaceViewport.h>
#include <App/Bitmap.h>

// TODO: obviously this shouldn't be a global const
const Size g_ViewportSize { .width = 800, .height = 600 };

typedef int WorkspaceIdentifier;

class Workspace {
private:
    WorkspaceIdentifier m_Identifier;
    WorkspaceViewport m_Viewport;
    Bitmap m_Bitmap; // This will get replaced with a layer/frame system

    explicit Workspace(WorkspaceIdentifier identifier)
        : m_Identifier(identifier)
        , m_Viewport(g_ViewportSize)
        , m_Bitmap(g_ViewportSize)
        { }

public:

    ~Workspace() {
        Logger::Core->debug("Closing workspace {}", m_Identifier);
    };

    [[nodiscard]] WorkspaceIdentifier GetIdentifier() const { return m_Identifier; }
    [[nodiscard]] WorkspaceViewport& GetViewport() { return m_Viewport; }

    static std::shared_ptr<Workspace> Create();
};
