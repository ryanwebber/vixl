#include <Core/Scene.h>

namespace Core {
    void Scene::Update(Core::UpdateSystem &system) {
        system.Update();
    }
}
