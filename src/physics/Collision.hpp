#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <raylib.h>
#include "../entities/Player.hpp"
#include "../entities/Platform.hpp"

// Clase estática para manejar colisiones
class Collision {
public:
    // Detección AABB básica entre dos rectángulos
    static bool checkAABB(const Rectangle& rect1, const Rectangle& rect2);
    
    // Verificar si un punto está dentro de un rectángulo
    static bool pointInRect(float x, float y, const Rectangle& rect);
    
    // Calcular overlap entre dos rectángulos en cada eje
    static float getOverlapX(const Rectangle& rect1, const Rectangle& rect2);
    static float getOverlapY(const Rectangle& rect1, const Rectangle& rect2);
    
    // Resolver colisión entre jugador y plataforma
    static void resolvePlayerPlatformCollision(Player& player, const Platform& platform);
    
    // Verificar si jugador está cayendo sobre la plataforma (desde arriba)
    static bool isFallingOnto(const Player& player, const Platform& platform);
    
    // Calcular la distancia entre dos puntos
    static float distance(float x1, float y1, float x2, float y2);
    
private:
    // Umbral mínimo de overlap para considerar colisión
    static constexpr float COLLISION_THRESHOLD = 0.1f;
};

#endif // COLLISION_HPP