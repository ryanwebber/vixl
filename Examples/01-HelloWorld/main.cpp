#include <VX/Core/Application.h>
#include <VX/Core/Async.h>
#include <VX/Core/Component/SpriteComponent.h>
#include <VX/Core/Component/TransformComponent.h>
#include <VX/Core/Logger.h>
#include <VX/Core/Platform/Platform.h>
#include <VX/Core/ResourceManager.h>
#include <VX/Core/Scene.h>
#include <VX/Core/SceneManager.h>
#include <VX/Core/SceneRenderer.h>
#include <VX/Core/System/SpriteRenderingSystem.h>

#include <entt/entity/registry.hpp>

#ifndef TARGET_FPS
    #define TARGET_FPS 60
#endif

int main()
{
    auto logger = VX::Logger::create_named("myapp");
    logger->debug("Running hello world example");

    VX::Core::ResourceLocator resource_locator(VX::Core::Platform::Current::get_resource_directory());
    std::shared_ptr<VX::Core::RenderBuiltins> builtins = VX::Core::RenderBuiltins::load_sync(resource_locator, "builtins.asset");

    VX::Core::ApplicationSettings app_settings {
        .window_size = { .width = 800, .height = 600 },
        .resource_directory = VX::Core::Platform::Current::get_resource_directory(),
    };

    // Create an application, which opens up a native window
    auto app = VX::Core::Application::create_from_settings(app_settings).value();

    // Create a new scene renderer and add it to the render stack
    auto scene_renderer = std::make_shared<VX::Core::SceneRenderer>(builtins);
    app->renderer().render_stack().add_layer(scene_renderer);

    // Create a render context for our scene to render into
    auto render_context = scene_renderer->create_render_context();

    // Create a new scene manager that will manage our various scenes
    VX::Core::SceneManager scene_manager;

    // Create a scene and set it as the active scene
    auto scene = VX::Core::Scene::create_named("Main");
    scene_manager.set_active_scene(scene);

    // Add some systems to the scene
    scene->render_systems().push_back(std::make_shared<VX::Core::System::SpriteRenderingSystem>(*builtins));

    // Create an etity in the scene
    auto entity = scene->entities().create();

    // Create a sprite component and transform component for the entity
    auto texture_handle = builtins->get_texture(VX::Core::Textures::UVMap).texture_handle();
    auto uniform_handle = std::make_shared<VX::Core::UniformHandle>(bgfx::createUniform("s_texColor",  bgfx::UniformType::Sampler));
    auto sprite_material = std::make_shared<VX::Core::Material>(builtins->get_material(VX::Core::Materials::Sprite).clone());
    sprite_material->set_texture<0>({ uniform_handle, texture_handle });
    scene->entities().emplace<VX::Core::Component::SpriteComponent>(entity, std::move(sprite_material));
    scene->entities().emplace<VX::Core::Component::TransformComponent>(entity);

    // Configure a main loop that runs at a target FPS
    static_assert(TARGET_FPS > 0, "Invalid target FPS");
    auto mills_per_frame = VX::Core::Millis(1000 / TARGET_FPS);
    auto render_timer = VX::Core::Time::Timer(app->event_loop().executor(), { }, mills_per_frame, false);

    // Process events every main tick
    auto process_input_handle = render_timer.subscriber().on([&](auto) {
        auto cs = app->input().process_events();
        if (cs == VX::Core::ControlState::Terminate) {
            app->terminate();
        }
    });

    // Update the scene and render a frame every main tick
    auto render_handle = render_timer.subscriber().on([&](auto _) {
        // Update the scene and render it into our target
        scene_manager.update();
        scene_manager.render(*render_context);

        // Present the rendered scene onto our window
        app->renderer().render_frame();
        app->window().native_window().swap_buffers();
    });

    // Start the main loop timer
    render_timer.start();

    // Start the application event loop
    app->run();
}
