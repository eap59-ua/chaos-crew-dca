#include "SpikeFactory.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Obstacle.hpp"
#include "../components/Velocity.hpp"
#include "../components/Sprite.hpp"

entt::entity createSpike(entt::registry& registry,
                         float x, float y, float width, float height, Texture2D texture)
{
    auto e = registry.create();
    registry.emplace<Position>(e, x, y);
    registry.emplace<Solid>(e, width, height);
    registry.emplace<Velocity>(e, 0.0f, 0.0f);
    registry.emplace<Obstacle>(e);  // Las trampas ya funcionan con TrapSystem

    float singleSpikeWidth = texture.width/2.0f; // Ancho de un pincho individual
    float scale = width / singleSpikeWidth;

    //Si la textura no cargó bien (width=0), evitamos dividir por cero.
    if (singleSpikeWidth == 0) { scale = 1.0f; singleSpikeWidth = 16.0f; }

    registry.emplace<Sprite>(e, 
        texture,                // texture
        Vector2{0,0},           // offset 
        singleSpikeWidth,       // frameWidth
        (float)texture.height,  // frameHeight
        scale,                   // scale 
        false,                  // flipX 
        1,                      // totalFrames
        0,                      // currentFrame
        0.0f,                   // frameTime
        0.0f                    // timer
    );
    return e;
}
