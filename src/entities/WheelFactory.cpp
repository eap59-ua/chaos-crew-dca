#include "WheelFactory.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Trap.hpp"

entt::entity createWheel(entt::registry& registry,
                         float x, float y, float radius, Color color)
{
    auto wheelEntity = registry.create();
    registry.emplace<Position>(wheelEntity, x, y);
    registry.emplace<Solid>(wheelEntity, radius*2, radius*2, color);
    registry.emplace<Trap>(wheelEntity);
    return wheelEntity;
}
