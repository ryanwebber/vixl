#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace VX::Core {
    class SceneCamera final {
    private:
        glm::mat4x4 m_view;
        glm::mat4x4 m_projection;

    public:
        SceneCamera(glm::mat4x4 view, glm::mat4x4 projection)
            : m_view(view)
            , m_projection(projection)
            {
            };

        [[nodiscard]] const glm::mat4x4& view_matrix() const { return m_view; }
        [[nodiscard]] const glm::mat4x4& projection_matrix() const { return m_projection; }
    };
}
