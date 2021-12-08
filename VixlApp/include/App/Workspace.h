#pragma once

#include <glm/glm.hpp>

#include <App/Logger.h>
#include <App/WorkspaceViewport.h>

typedef int WorkspaceIdentifier;

class Workspace {
private:
    WorkspaceIdentifier m_Identifier;
    WorkspaceViewport m_Viewport;

    explicit Workspace(WorkspaceIdentifier identifier)
        : m_Identifier(identifier)
        , m_Viewport({ 800, 600 })
        { }

public:

    ~Workspace() {
        Logger::Core->debug("Closing workspace {}", m_Identifier);
    };

    [[nodiscard]] WorkspaceIdentifier GetIdentifier() const { return m_Identifier; }
    [[nodiscard]] WorkspaceViewport& GetViewport() { return m_Viewport; }

    static std::shared_ptr<Workspace> Create();
};
