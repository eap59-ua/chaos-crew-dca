#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "../entities/Player.hpp"
#include "../entities/Platform.hpp"
#include <vector>

// Constantes de física globales
namespace Physics {
    constexpr float GRAVITY = 0.6f;           // Aceleración gravitatoria (units/frame²)
    constexpr float TERMINAL_VELOCITY = 20.0f; // Velocidad máxima de caída
    constexpr float GROUND_FRICTION = 0.8f;    // Fricción del suelo (Hito 2+)
    constexpr float AIR_RESISTANCE = 0.98f;    // Resistencia del aire (Hito 2+)
}

// Motor de físicas del juego
class PhysicsEngine {
public:
    PhysicsEngine();
    
    // Aplicar gravedad
    void applyGravity(Player& player, float deltaTime);
    void applyGravityToAll(std::vector<Player>& players, float deltaTime);
    
    // Actualizar física de jugadores
    void updatePlayer(Player& player, float deltaTime);
    void updateAllPlayers(std::vector<Player>& players, float deltaTime);
    
    // Procesar colisiones
    void processCollisions(Player& player, const std::vector<Platform>& platforms);
    void processAllCollisions(std::vector<Player>& players, const std::vector<Platform>& platforms);
    
    // Verificar colisión con zona de salida
    bool checkExitCollision(const Player& player, const ExitZone& exitZone);
    
    // Configuración
    void setGravity(float newGravity) { gravity = newGravity; }
    float getGravity() const { return gravity; }
    
private:
    float gravity;
};

#endif // PHYSICS_HPP