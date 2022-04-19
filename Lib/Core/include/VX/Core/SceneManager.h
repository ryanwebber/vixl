#pragma once

#include <VX/Core/Renderer.h>
#include <VX/Core/Scene.h>

namespace VX::Core {
    class SceneManager final {
    private:
        std::shared_ptr<Scene> m_current_scene { };
    public:
        explicit SceneManager() = default;
        ~SceneManager() = default;

        [[nodiscard]] const std::shared_ptr<Scene> &current_scene() const { return m_current_scene; }
        void set_current_scene(std::shared_ptr<Scene> current_scene);

        void update() const;
    };
}
