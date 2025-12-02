#include "WheelFactory.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Obstacle.hpp"
#include "../components/Velocity.hpp"
#include "../components/Sprite.hpp"

entt::entity createWheel(entt::registry& registry,
                         float x, float y, float radius, Texture2D texture)
{
    auto e = registry.create();
    float diameter = radius * 2.0f;

    registry.emplace<Position>(e, x, y);
    registry.emplace<Solid>(e, diameter, diameter);
    registry.emplace<Velocity>(e, 0.0f, 0.0f);
    registry.emplace<Obstacle>(e);

    registry.emplace<Sprite>(e, 
        texture,       
        Vector2{0,0},   // offset
        54.0f,          // frameWidth
        52.0f,          // frameHeight
        1.2f,           // scale (Ajusta tamaño si quieres)
        false,          // flipX
        4,              // totalFrames
        0,              // currentFrame
        0.15f,          // frameTime (Velocidad de animación)
        0.0f            // timer
    );
    return e;
}
