#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <App/Size.h>

class Camera final {
private:
    glm::vec3 m_Position;
    float m_Scale;

public:
    Camera()
        : m_Position(0.25f, 0.0, 0.0)
        , m_Scale(0.5f)
    { };

    ~Camera() = default;

    [[nodiscard]] glm::mat4 GetProjection(Size size) const {

        // Scale matrix
        auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, m_Scale, 1.0f));

        // Projection matrix
        auto projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

        // View matrix
        auto view = glm::lookAt(m_Position, glm::vec3(0.0f, 0.0, 1.0), glm::vec3(0.0f, 1.0f, 0.0f));

        return (projection * scale) * view;
    }
};
