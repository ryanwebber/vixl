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
    SizeInt m_Size;
    Bitmap m_Bitmap; // This will get replaced with a layer/frame system

    explicit Workspace(WorkspaceIdentifier identifier, SizeInt size)
        : m_Identifier(identifier)
        , m_Size(size)
        , m_Bitmap(size)
        { }

public:

    ~Workspace() {
        Logger::Core->debug("Closing workspace {}", m_Identifier);
    };

    [[nodiscard]] WorkspaceIdentifier GetIdentifier() const { return m_Identifier; }
    [[nodiscard]] Bitmap& GetBitmap() { return m_Bitmap; }

    [[nodiscard]] SizeInt GetSize() const { return m_Size; }

    static std::shared_ptr<Workspace> Create(SizeInt size);
};
