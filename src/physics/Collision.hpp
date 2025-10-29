#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "../entities/Player.hpp"
#include "../entities/Platform.hpp"
#include <raylib.h>

// Utilidades para detección y resolución de colisiones
class Collision {
public:
    // Threshold mínimo para considerar una colisión
    static constexpr float COLLISION_THRESHOLD = 1.0f;
    
    // Detección AABB (Axis-Aligned Bounding Box)
    static bool checkAABB(const Rectangle& rect1, const Rectangle& rect2);
    
    // Verificar si un punto está dentro de un rectángulo
    static bool pointInRect(float x, float y, const Rectangle& rect);
    
    // Calcular overlap en cada eje
    static float getOverlapX(const Rectangle& rect1, const Rectangle& rect2);
    static float getOverlapY(const Rectangle& rect1, const Rectangle& rect2);
    
    // Verificar si jugador está cayendo sobre plataforma
    static bool isFallingOnto(const Player& player, const Platform& platform);
    
    // Resolver colisión entre jugador y plataforma
    static void resolvePlayerPlatformCollision(Player& player, const Platform& platform);
    
    // Utilidades geométricas
    static float distance(float x1, float y1, float x2, float y2);
};

#endif // COLLISION_HPP