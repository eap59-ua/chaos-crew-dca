#include "SpikeFactory.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Obstacle.hpp"
#include "../components/Velocity.hpp"

entt::entity createSpike(entt::registry& registry,
                         float x, float y, float width, float height, Color color)
{
    auto e = registry.create();
    registry.emplace<Position>(e, x, y);
    registry.emplace<Solid>(e, width, height, color);
    registry.emplace<Velocity>(e, 0.0f, 0.0f);
    registry.emplace<Obstacle>(e);  // Las trampas ya funcionan con TrapSystem
    return e;
}
