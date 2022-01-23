#include <VX/Core/Component/TransformComponent.h>
#include <VX/Core/Component/SpriteComponent.h>
#include <VX/Core/System/SpriteRenderingSystem.h>

namespace VX::Core::System {

    SpriteRenderingSystem::SpriteRenderingSystem(const RenderBuiltins &builtins)
        : m_quad_shape(builtins.get_shape(Shapes::Quad))
    {
    }

    void SpriteRenderingSystem::render(RenderBuffer &buffer, entt::registry &entities) {
        auto view = entities.view<const Component::TransformComponent, const Component::SpriteComponent>();
        for(auto [_, transform, sprite]: view.each()) {
            buffer.draw_texture_quad(transform.transform(), sprite.material);
        }
    }
}
