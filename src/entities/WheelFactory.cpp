#include "WheelFactory.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Obstacle.hpp"
#include "../components/Velocity.hpp"

entt::entity createWheel(entt::registry& registry,
                         float x, float y, float radius, Color color)
{
    auto wheelEntity = registry.create();
    registry.emplace<Position>(wheelEntity, x, y);
    registry.emplace<Solid>(wheelEntity, radius*2, radius*2, color);
    registry.emplace<Velocity>(wheelEntity, 0.0f, 0.0f);
    registry.emplace<Obstacle>(wheelEntity);
    return wheelEntity;
}
