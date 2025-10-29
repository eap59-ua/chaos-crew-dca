#include "Collision.hpp"
#include <cmath>
#include <algorithm>

// Detección AABB (Axis-Aligned Bounding Box)
bool Collision::checkAABB(const Rectangle& rect1, const Rectangle& rect2) {
    return (rect1.x < rect2.x + rect2.width &&
            rect1.x + rect1.width > rect2.x &&
            rect1.y < rect2.y + rect2.height &&
            rect1.y + rect1.height > rect2.y);
}

// Verificar si un punto está dentro de un rectángulo
bool Collision::pointInRect(float x, float y, const Rectangle& rect) {
    return (x >= rect.x && x <= rect.x + rect.width &&
            y >= rect.y && y <= rect.y + rect.height);
}

// Calcular overlap en X
float Collision::getOverlapX(const Rectangle& rect1, const Rectangle& rect2) {
    float left = std::max(rect1.x, rect2.x);
    float right = std::min(rect1.x + rect1.width, rect2.x + rect2.width);
    return std::max(0.0f, right - left);
}

// Calcular overlap en Y
float Collision::getOverlapY(const Rectangle& rect1, const Rectangle& rect2) {
    float top = std::max(rect1.y, rect2.y);
    float bottom = std::min(rect1.y + rect1.height, rect2.y + rect2.height);
    return std::max(0.0f, bottom - top);
}

// Verificar si jugador está cayendo sobre plataforma
bool Collision::isFallingOnto(const Player& player, const Platform& platform) {
    // El jugador está cayendo si su velocidad vertical es positiva (hacia abajo)
    // Y si la parte inferior del jugador está cerca de la parte superior de la plataforma
    
    float playerBottom = player.position.y + player.size.y;
    float platformTop = platform.position.y;
    
    // Verificar que el jugador está cayendo
    bool isFalling = player.velocity.y > 0;
    
    // Verificar que la parte inferior del jugador está cerca/dentro de la plataforma
    bool isNearTop = playerBottom >= platformTop && playerBottom <= platformTop + 15.0f;
    
    return isFalling && isNearTop;
}

// Resolver colisión entre jugador y plataforma
void Collision::resolvePlayerPlatformCollision(Player& player, const Platform& platform) {
    Rectangle playerRect = player.getCollisionRect();
    Rectangle platformRect = platform.getCollisionRect();
    
    // Calcular overlap en cada eje
    float overlapX = getOverlapX(playerRect, platformRect);
    float overlapY = getOverlapY(playerRect, platformRect);
    
    // Si no hay overlap suficiente, no hay colisión
    if (overlapX < COLLISION_THRESHOLD || overlapY < COLLISION_THRESHOLD) {
        return;
    }
    
    // Determinar dirección de resolución basándose en el overlap menor
    // Esto evita que el jugador "salte" a través de plataformas
    
    if (overlapY < overlapX) {
        // Colisión vertical (desde arriba o abajo)
        
        if (isFallingOnto(player, platform)) {
            // Jugador cayendo sobre plataforma desde arriba
            player.position.y = platform.position.y - player.size.y;
            player.velocity.y = 0;
            player.isGrounded = true;
        } else if (player.velocity.y < 0) {
            // Jugador golpeando plataforma desde abajo
            player.position.y = platform.position.y + platform.size.y;
            player.velocity.y = 0;
        }
    } else {
        // Colisión horizontal (desde los lados)
        // En este caso, empujar al jugador hacia fuera horizontalmente
        
        float playerCenterX = player.position.x + player.size.x / 2;
        float platformCenterX = platform.position.x + platform.size.x / 2;
        
        if (playerCenterX < platformCenterX) {
            // Jugador a la izquierda de la plataforma
            player.position.x = platform.position.x - player.size.x;
        } else {
            // Jugador a la derecha de la plataforma
            player.position.x = platform.position.x + platform.size.x;
        }
        
        player.velocity.x = 0;
    }
}

// Calcular distancia entre dos puntos
float Collision::distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}