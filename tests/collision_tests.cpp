#define BOOST_TEST_MODULE CollisionTests_ChaosCrew
#include <boost/test/included/unit_test.hpp>
#include "../src/entt/entt.hpp"
#include "../src/components/Position.hpp"
#include "../src/components/Velocity.hpp"
#include "../src/components/Solid.hpp"
#include "../src/components/Player.hpp"
#include "../src/components/Platform.hpp"
#include "../src/components/Obstacle.hpp"
#include <cmath>

// ============================================================================
// Helper: Detectar colisión AABB (como CheckCollisionRecs de Raylib)
// ============================================================================
struct Rectangle {
    float x, y, width, height;
};

bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2) {
    return !(rec1.x + rec1.width <= rec2.x ||
             rec1.x >= rec2.x + rec2.width ||
             rec1.y + rec1.height <= rec2.y ||
             rec1.y >= rec2.y + rec2.height);
}

// ============================================================================
// TEST #8: Detectar colisión entre jugador y plataforma (CASO 7: Colisiones)
// ============================================================================
// ¿Qué testea? Que se detectan colisiones AABB básicas
// ¿Por qué? Es la base del CollisionSystem de Chaos Crew
// Teoría: Unit test, BlackBox (solo verificamos detección)

BOOST_AUTO_TEST_CASE(test_detect_player_platform_collision) {
    // ===== ARRANGE =====
    entt::registry registry;
    
    // Crear jugador
    auto playerEntity = registry.create();
    auto& playerPos = registry.emplace<Position>(playerEntity);
    playerPos.x = 100.0f;
    playerPos.y = 100.0f;
    
    auto& playerSolid = registry.emplace<Solid>(playerEntity);
    playerSolid.width = 32.0f;
    playerSolid.height = 32.0f;
    
    // Crear plataforma que se solapa con el jugador
    auto platformEntity = registry.create();
    auto& platformPos = registry.emplace<Position>(platformEntity);
    platformPos.x = 110.0f;  // Se solapa 22px horizontalmente
    platformPos.y = 110.0f;  // Se solapa 22px verticalmente
    
    auto& platformSolid = registry.emplace<Solid>(platformEntity);
    platformSolid.width = 64.0f;
    platformSolid.height = 32.0f;
    
    registry.emplace<Platform>(platformEntity);
    
    // ===== ACT =====
    Rectangle playerRect = {playerPos.x, playerPos.y, playerSolid.width, playerSolid.height};
    Rectangle platformRect = {platformPos.x, platformPos.y, platformSolid.width, platformSolid.height};
    
    bool collision = CheckCollisionRecs(playerRect, platformRect);
    
    // ===== ASSERT =====
    BOOST_TEST(collision, "Player and platform should collide");
    
    // Verificar simetría (A vs B = B vs A)
    bool collisionReverse = CheckCollisionRecs(platformRect, playerRect);
    BOOST_TEST(collisionReverse, "Collision should be symmetric");
}

// ============================================================================
// TEST #9: Calcular penetración y resolver colisión (CASO 7: Física)
// ============================================================================
// ¿Qué testea? Que se calcula correctamente la penetración para resolver colisiones
// ¿Por qué? CollisionSystem usa esto para separar jugador y plataforma
// Teoría: Unit test, WhiteBox (conocemos el algoritmo de CollisionSystem)

BOOST_AUTO_TEST_CASE(test_calculate_collision_penetration) {
    // ===== ARRANGE =====
    // Jugador cayendo sobre una plataforma
    Position playerPos = {100.0f, 150.0f};  // Jugador arriba
    Solid playerSolid = {32.0f, 32.0f};
    
    Position platformPos = {100.0f, 180.0f};  // Plataforma abajo
    Solid platformSolid = {64.0f, 16.0f};
    
    // Jugador y plataforma se solapan verticalmente
    // Player: y=150, height=32 → bottom=182
    // Platform: y=180, height=16 → top=180
    // Overlap bottom: (150 + 32) - 180 = 2px
    
    Velocity playerVel = {0.0f, 10.0f};  // Cayendo (velocidad positiva)
    
    // ===== ACT =====
    // Calcular solapamientos (como en CollisionSystem.cpp líneas 36-40)
    float overlapTop    = (platformPos.y + platformSolid.height) - playerPos.y;
    float overlapBottom = (playerPos.y + playerSolid.height) - platformPos.y;
    float overlapLeft   = (platformPos.x + platformSolid.width) - playerPos.x;
    float overlapRight  = (playerPos.x + playerSolid.width) - platformPos.x;
    
    // Encontrar penetración mínima
    float minOverlap = fminf(fminf(overlapTop, overlapBottom),
                             fminf(overlapLeft, overlapRight));
    
    // ===== ASSERT =====
    // La penetración mínima es por abajo (2px)
    BOOST_CHECK_CLOSE(overlapBottom, 2.0f, 0.001f);
    BOOST_CHECK_CLOSE(minOverlap, overlapBottom, 0.001f);
    
    
    bool isLandingOnPlatform = (minOverlap == overlapBottom) && (playerVel.vy >= 0);
    BOOST_TEST(isLandingOnPlatform, "Should detect landing on platform");
    
    // Resolver colisión: mover jugador encima de la plataforma
    float correctedY = platformPos.y - playerSolid.height;
    BOOST_CHECK_CLOSE(correctedY, 148.0f, 0.001f);  // 180 - 32 = 148
}

// ============================================================================
// NOTAS TEÓRICAS - Sistema de Colisiones en Chaos Crew
// ============================================================================
/*
Algoritmo de colisiones AABB (Axis-Aligned Bounding Box):

Fórmula de detección:
```
NO_COLISION = (A.right <= B.left) ||
              (A.left >= B.right) ||
              (A.bottom <= B.top) ||
              (A.top >= B.bottom)

COLISION = !NO_COLISION
```

Visualización:
```
     Player
     +------+
     |      |
     |  +---|----+  Platform
     +--|---+    |
        |        |
        +--------+
```

Cálculo de penetración (CollisionSystem.cpp):
1. Calcular 4 solapamientos:
   - overlapTop: Cuánto penetra por arriba
   - overlapBottom: Cuánto penetra por abajo
   - overlapLeft: Cuánto penetra por izquierda
   - overlapRight: Cuánto penetra por derecha

2. Encontrar la penetración MÍNIMA:
   ```cpp
   float minOverlap = fminf(fminf(overlapTop, overlapBottom),
                            fminf(overlapLeft, overlapRight));
   ```

3. Resolver según la penetración mínima:
   ```cpp
   if (minOverlap == overlapBottom && vel.vy >= 0) {
       // Colisión desde arriba (aterrizar)
       pos.y = platformY - playerHeight;
       vel.vy = 0;
       player.onGround = true;
   }
   ```

Tipos de colisiones en Chaos Crew:

1. **Jugador vs Plataforma**:
   - Desde arriba → aterrizar (onGround = true)
   - Desde abajo → golpear techo
   - Desde lados → chocar contra pared

2. **Jugador vs Trampa (Obstacle)**:
   - Desde arriba → MUERTE (isDead = true)
   - Desde otros lados → comportamiento sólido (no mata)

3. **Jugador vs Rueda**:
   - Cualquier dirección → MUERTE inmediata

Características del sistema:
✅ Basado en AABB (rápido y simple)
✅ Resolución por penetración mínima
✅ Detección de dirección de colisión
✅ Diferentes respuestas según tipo de objeto
✅ Establece flags (onGround, isDead)

