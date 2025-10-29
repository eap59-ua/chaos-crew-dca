#include "Physics.hpp"
#include "Collision.hpp"

PhysicsEngine::PhysicsEngine()
    : gravity(Physics::GRAVITY)
{
}

// Aplicar gravedad a un jugador
void PhysicsEngine::applyGravity(Player& player, float deltaTime) {
    player.applyGravity(gravity);
}

// Aplicar gravedad a todos los jugadores
void PhysicsEngine::applyGravityToAll(std::vector<Player>& players, float deltaTime) {
    for (auto& player : players) {
        applyGravity(player, deltaTime);
    }
}


// Actualizar físicas de un jugador
void PhysicsEngine::updatePlayer(Player& player, float deltaTime) {
    // CRÍTICO: Resetear isGrounded ANTES de aplicar física
    player.isGrounded = false;
    
    // Aplicar gravedad
    applyGravity(player, deltaTime);
    
    // Actualizar posición
    player.updatePosition(deltaTime);
}
// Actualizar físicas de todos los jugadores
void PhysicsEngine::updateAllPlayers(std::vector<Player>& players, float deltaTime) {
    for (auto& player : players) {
        updatePlayer(player, deltaTime);
    }
}

// Procesar colisiones de un jugador con todas las plataformas
void PhysicsEngine::processCollisions(Player& player, const std::vector<Platform>& platforms) {
    if (!player.isAlive) return;
    
    // Asumir que no está en el suelo hasta que se demuestre lo contrario
    //player.isGrounded = false;
    
    Rectangle playerRect = player.getCollisionRect();
    
    for (const auto& platform : platforms) {
        if (!platform.isActive) continue;
        
        Rectangle platformRect = platform.getCollisionRect();
        
        // Verificar colisión AABB
        if (Collision::checkAABB(playerRect, platformRect)) {
            // Resolver colisión y actualizar estado
            Collision::resolvePlayerPlatformCollision(player, platform);
        }
    }
}

// Procesar colisiones de todos los jugadores
void PhysicsEngine::processAllCollisions(std::vector<Player>& players, const std::vector<Platform>& platforms) {
    for (auto& player : players) {
        processCollisions(player, platforms);
    }
}

// Verificar si jugador está en zona de salida
bool PhysicsEngine::checkExitCollision(const Player& player, const ExitZone& exitZone) {
    if (!player.isAlive) return false;
    
    Rectangle playerRect = player.getCollisionRect();
    return Collision::checkAABB(playerRect, exitZone.rect);
}