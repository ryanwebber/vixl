#pragma once

#include <glm/glm.hpp>

#include <App/Size.h>

class WorkspaceViewport final {
private:
    Size m_Size;
    void* m_Texture { nullptr };

public:

    explicit WorkspaceViewport(Size size)
        : m_Size(size)
    { }

    ~WorkspaceViewport() = default;

    [[nodiscard]] void* GetTexture() const { return m_Texture; }
    void SetTexture(void* texture) { m_Texture = texture; }

    [[nodiscard]] Size GetSize() const { return m_Size; }
    void SetSize(const Size& size) { m_Size = size; }
};
