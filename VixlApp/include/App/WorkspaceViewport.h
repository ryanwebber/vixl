#pragma once

#include <glm/glm.hpp>

class WorkspaceViewport final {
private:
    glm::vec2 m_Size;
    void* m_Texture;

public:
    explicit WorkspaceViewport(glm::vec2 size)
        : m_Size(size)
        , m_Texture(nullptr)
    { }

    ~WorkspaceViewport() = default;

    [[nodiscard]] void* GetTexture() const { return m_Texture; }
    void SetTexture(void* texture) { m_Texture = texture; }

    [[nodiscard]] glm::vec2 GetSize() const { return m_Size; }
    void SetSize(const glm::vec2& size) { m_Size = size; }
};
