#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <Core/Size.h>

namespace Core {

    class Camera final {
    private:
        glm::vec2 m_Position;
        float m_Scale;

    public:
        Camera()
                : m_Position(0.0, 0.0), m_Scale(1.0) {};

        ~Camera() = default;

        float &Scale() { return m_Scale; }

        [[nodiscard]] float GetScale() const { return m_Scale; }

        glm::vec2 &Position() { return m_Position; }

        [[nodiscard]] glm::vec2 GetPosition() const { return m_Position; }

        [[nodiscard]] glm::mat4 GetProjection() const {

            // Scale matrix
            auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, m_Scale, 1.0f));

            // Projection matrix
            auto projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

            // View matrix
            auto view = glm::lookAt(glm::vec3(m_Position.x, m_Position.y, 0.0f), glm::vec3(0.0f, 0.0, 1.0),
                                    glm::vec3(0.0f, 1.0f, 0.0f));

            return (projection * scale) * view;
        }
    };
}