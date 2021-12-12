#pragma once

#include <glm/glm.hpp>

#include <Common/Copyable.h>

#include <App/Size.h>
#include <App/Camera.h>

class WorkspaceViewport final {
    VX_DEFAULT_COPYABLE(WorkspaceViewport);
    VX_DEFAULT_MOVABLE(WorkspaceViewport);

private:
    Size m_Size;
    Camera m_Camera;

public:

    explicit WorkspaceViewport(Size size)
        : m_Size(size)
    { }

    ~WorkspaceViewport() = default;

    [[nodiscard]] Size GetSize() const { return m_Size; }
    void SetSize(const Size& size) { m_Size = size; }

    Camera &GetCamera() { return m_Camera; }
    [[nodiscard]] const Camera &GetCamera() const { return m_Camera; }
};
