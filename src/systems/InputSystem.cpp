#include <raylib.h>
#include "../entt/entt.hpp"
#include "../components/Player.hpp"
#include "../components/Mandos.hpp"

void InputSystem(entt::registry& registry) {
    auto view = registry.view<Player, Mandos>();

    for (auto entity : view) {
        auto& player = view.get<Player>(entity);
        auto& mando = view.get<Mandos>(entity);

        player.left = IsKeyDown(mando.left);
        player.right = IsKeyDown(mando.right);
        player.jump = IsKeyPressed(mando.jump);
    }
}
