#include "PatronSystem.hpp"
#include "../components/Patron.hpp"
#include "../entt/entt.hpp"

void PatronSystem(entt::registry& registry, float dt)
{
    auto view = registry.view<Patron>();

    for (auto e : view)
    {
        auto &patron = view.get<Patron>(e);

        for (auto &action : patron.actions)
            action(dt);      
    }
}
