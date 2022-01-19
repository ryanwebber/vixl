#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace VX::Core {
    class SceneCamera final {
    private:
        glm::mat4x4 m_View;
        glm::mat4x4 m_Projection;

    public:
        SceneCamera(glm::mat4x4 view, glm::mat4x4 projection)
            : m_View(view)
            , m_Projection(projection)
            {
            };

        [[nodiscard]] const glm::mat4x4& GetViewMatrix() const { return m_View; }
        [[nodiscard]] const glm::mat4x4& GetProjectionMatrix() const { return m_Projection; }
    };
}
