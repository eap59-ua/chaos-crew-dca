#include "Platform.hpp"

// Constructor simple
Platform::Platform(Vector2 pos, Vector2 platformSize, Color platformColor)
    : position(pos)
    , size(platformSize)
    , color(platformColor)
    , type(PlatformType::STATIC)
    , isActive(true)
    , moveDirection({0.0f, 0.0f})
    , moveSpeed(0.0f)
    , startPos(pos)
    , endPos(pos)
{
}

// Constructor completo
Platform::Platform(Vector2 pos, Vector2 platformSize, Color platformColor, PlatformType platformType)
    : position(pos)
    , size(platformSize)
    , color(platformColor)
    , type(platformType)
    , isActive(true)
    , moveDirection({0.0f, 0.0f})
    , moveSpeed(0.0f)
    , startPos(pos)
    , endPos(pos)
{
}

// Actualizar plataforma (para plataformas móviles en el futuro)
void Platform::update(float deltaTime) {
    if (!isActive) return;
    
    if (type == PlatformType::MOVING) {
        // TODO: Implementar movimiento en Hito 2
        // position.x += moveDirection.x * moveSpeed * deltaTime;
        // position.y += moveDirection.y * moveSpeed * deltaTime;
    }
}

// Renderizar plataforma
void Platform::render() const {
    if (!isActive) return;
    
    // Dibujar plataforma
    DrawRectangleV(position, size, color);
    
    // Dibujar borde
    DrawRectangleLinesEx({position.x, position.y, size.x, size.y}, 1, DARKGRAY);
}

// Obtener rectángulo de colisión
Rectangle Platform::getCollisionRect() const {
    return {position.x, position.y, size.x, size.y};
}

// Crear suelo principal
Platform Platform::createGround(float screenWidth) {
    return Platform(
        {0.0f, 670.0f},           // Posición: abajo de la pantalla
        {screenWidth, 50.0f},     // Tamaño: ancho completo, 50px alto
        DARKGRAY                  // Color gris oscuro
    );
}

// Crear plataforma normal
Platform Platform::createNormalPlatform(float x, float y, float width) {
    return Platform(
        {x, y},
        {width, 20.0f},  // Altura estándar de 20px
        GRAY
    );
}

// ============================================
// IMPLEMENTACIÓN DE EXITZONE
// ============================================

ExitZone::ExitZone(float x, float y, float width, float height)
    : rect({x, y, width, height})
    , color(GREEN)
    , player1Inside(false)
    , player2Inside(false)
{
}

void ExitZone::render() const {
    // Dibujar zona de salida
    DrawRectangleRec(rect, color);
    
    // Dibujar borde más grueso
    DrawRectangleLinesEx(rect, 3, DARKGREEN);
    
    // Texto "EXIT"
    const char* text = "EXIT";
    int textWidth = MeasureText(text, 30);
    DrawText(text,
             rect.x + (rect.width - textWidth) / 2,
             rect.y + (rect.height - 30) / 2,
             30,
             WHITE);
    
    // Indicadores visuales si los jugadores están dentro
    if (player1Inside) {
        DrawCircle(rect.x + 15, rect.y + rect.height - 15, 8, BLUE);
    }
    if (player2Inside) {
        DrawCircle(rect.x + rect.width - 15, rect.y + rect.height - 15, 8, RED);
    }
}

void ExitZone::reset() {
    player1Inside = false;
    player2Inside = false;
}

bool ExitZone::bothPlayersInside() const {
    return player1Inside && player2Inside;
}