#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <raylib.h>

enum class PlatformType {
    STATIC,          // Plataforma estática normal
    MOVING,          // Plataforma móvil (Hito 2+)
    DISAPPEARING,    // Plataforma que desaparece (Hito 2+)
    BREAKABLE        // Plataforma que se rompe (Hito 2+)
};

class Platform {
public:
    Vector2 position;
    Vector2 size;
    Color color;
    PlatformType type;
    bool isActive;
    
    // Para plataformas móviles (futuro)
    Vector2 moveDirection;
    float moveSpeed;
    Vector2 startPos;
    Vector2 endPos;
    
    // Constructor para plataforma estática
    Platform(Vector2 pos, Vector2 platformSize, Color platformColor = GRAY);
    
    // Constructor completo (para futuras expansiones)
    Platform(Vector2 pos, Vector2 platformSize, Color platformColor, PlatformType platformType);
    
    // Métodos
    void update(float deltaTime);
    void render() const;
    Rectangle getCollisionRect() const;
    
    // Métodos estáticos para crear plataformas comunes
    static Platform createGround(float screenWidth);
    static Platform createNormalPlatform(float x, float y, float width);
};

// Estructura para zona de salida (objetivo cooperativo)
struct ExitZone {
    Rectangle rect;
    Color color;
    bool player1Inside;
    bool player2Inside;
    
    ExitZone(float x, float y, float width, float height);
    
    void render() const;
    void reset();
    bool bothPlayersInside() const;
};

#endif // PLATFORM_HPP