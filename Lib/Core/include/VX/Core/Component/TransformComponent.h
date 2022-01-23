#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace VX::Core::Component {
    struct TransformComponent final {
        glm::vec3 translation;
        glm::vec3 scale { 1.0f, 1.0f, 1.0f };
        glm::vec3 rotation;

        [[nodiscard]] glm::mat4x4 transform() const {
            auto identity_matrix = glm::mat4(1.0f);
            auto translation_matrix = glm::translate(identity_matrix, translation);
            auto scale_matrix = glm::scale(identity_matrix, scale);
            auto rotation_matrix = glm::mat4_cast(glm::quat(rotation));
            return translation_matrix * rotation_matrix * scale_matrix;
        }
    };
}
