#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "../entities/Player.hpp"
#include "../entities/Platform.hpp"
#include <vector>

// Constantes de física del juego
namespace Physics {
    constexpr float GRAVITY = 0.6f;              // Aceleración gravitatoria
    constexpr float TERMINAL_VELOCITY = 20.0f;   // Velocidad máxima de caída
    constexpr float GROUND_FRICTION = 0.8f;      // Fricción en el suelo (futuro)
    constexpr float AIR_RESISTANCE = 0.98f;      // Resistencia del aire (futuro)
}

// Clase para manejar todas las físicas del juego
class PhysicsEngine {
public:
    PhysicsEngine();
    
    // Aplicar gravedad a un jugador
    void applyGravity(Player& player, float deltaTime = 1.0f);
    
    // Aplicar gravedad a múltiples jugadores
    void applyGravityToAll(std::vector<Player>& players, float deltaTime = 1.0f);
    
    // Actualizar físicas de un jugador
    void updatePlayer(Player& player, float deltaTime = 1.0f);
    
    // Actualizar físicas de todos los jugadores
    void updateAllPlayers(std::vector<Player>& players, float deltaTime = 1.0f);
    
    // Procesar colisiones de un jugador con todas las plataformas
    void processCollisions(Player& player, const std::vector<Platform>& platforms);
    
    // Procesar colisiones de todos los jugadores con todas las plataformas
    void processAllCollisions(std::vector<Player>& players, const std::vector<Platform>& platforms);
    
    // Verificar si jugador está en zona de salida
    bool checkExitCollision(const Player& player, const ExitZone& exitZone);
    
    // Getters
    float getGravity() const { return Physics::GRAVITY; }
    
private:
    float gravity;
};

#endif // PHYSICS_HPP