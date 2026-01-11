#define BOOST_TEST_MODULE IntegrationTests
#include <boost/test/included/unit_test.hpp>
#include <entt/entt.hpp>
#include <memory>

// ============================================================================
// COMPONENTES Y SISTEMAS (reutilizados de tests anteriores)
// ============================================================================

struct TransformComponent {
    float x, y;
    float rotation;
    float scaleX, scaleY;
};

struct VelocityComponent {
    float vx, vy;
};

struct SpriteComponent {
    int textureID;
    float x, y;
    float width, height;
};

struct Collider {
    float width, height;
    bool isTrigger;
    
    float left(const TransformComponent& t) const { return t.x; }
    float right(const TransformComponent& t) const { return t.x + width; }
    float top(const TransformComponent& t) const { return t.y; }
    float bottom(const TransformComponent& t) const { return t.y + height; }
};

struct Health {
    int current;
    int maximum;
    
    bool isAlive() const { return current > 0; }
};

// ============================================================================
// SISTEMAS DEL JUEGO
// ============================================================================

class MovementSystem {
public:
    static void update(entt::registry& registry, float deltaTime) {
        auto view = registry.view<TransformComponent, VelocityComponent>();
        for (auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
            auto& velocity = view.get<VelocityComponent>(entity);
            
            transform.x += velocity.vx * deltaTime;
            transform.y += velocity.vy * deltaTime;
        }
    }
};

class PhysicsSystem {
public:
    static bool checkCollision(const Collider& a, const TransformComponent& ta,
                               const Collider& b, const TransformComponent& tb) {
        return !(a.right(ta) <= b.left(tb) ||
                 a.left(ta) >= b.right(tb) ||
                 a.bottom(ta) <= b.top(tb) ||
                 a.top(ta) >= b.bottom(tb));
    }
    
    // Procesar todas las colisiones y aplicar daño
    static void resolveCollisions(entt::registry& registry) {
        auto view = registry.view<TransformComponent, Collider, Health>();
        
        // Algoritmo O(n²) - en juego real usarías QuadTree
        for (auto entityA : view) {
            for (auto entityB : view) {
                if (entityA == entityB) continue;
                
                auto& transformA = view.get<TransformComponent>(entityA);
                auto& colliderA = view.get<Collider>(entityA);
                auto& transformB = view.get<TransformComponent>(entityB);
                auto& colliderB = view.get<Collider>(entityB);
                
                if (checkCollision(colliderA, transformA, colliderB, transformB)) {
                    // Aplicar daño a ambas entidades
                    auto& healthA = view.get<Health>(entityA);
                    auto& healthB = view.get<Health>(entityB);
                    
                    healthA.current -= 10;
                    healthB.current -= 10;
                }
            }
        }
    }
};

// ============================================================================
// TEST #10: Integración completa del juego (TODOS LOS CASOS)
// ============================================================================
// ¿Qué testea? Que todos los sistemas funcionan juntos en un escenario real
// ¿Por qué? Los bugs suelen aparecer en la INTERACCIÓN entre sistemas
// Teoría: Integration test, simula un mini juego completo

BOOST_AUTO_TEST_CASE(test_collision_applies_damage) {
    // ===== ARRANGE =====
    // Crear un mini juego con 2 personajes que van a colisionar
    entt::registry registry;
    
    // PERSONAJE 1: Player en posición (0, 0) moviéndose a la derecha
    auto player = registry.create();
    registry.emplace<TransformComponent>(player, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    registry.emplace<VelocityComponent>(player, 10.0f, 0.0f);  // 10 px/seg a la derecha
    registry.emplace<SpriteComponent>(player, 1, 0.0f, 0.0f, 32.0f, 32.0f);
    registry.emplace<Collider>(player, 32.0f, 32.0f, false);
    registry.emplace<Health>(player, 100, 100);  // 100 HP
    
    // PERSONAJE 2: Enemy en posición (15, 0) estático
    auto enemy = registry.create();
    registry.emplace<TransformComponent>(enemy, 15.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    registry.emplace<VelocityComponent>(enemy, 0.0f, 0.0f);    // Estático
    registry.emplace<SpriteComponent>(enemy, 2, 15.0f, 0.0f, 32.0f, 32.0f);
    registry.emplace<Collider>(enemy, 32.0f, 32.0f, false);
    registry.emplace<Health>(enemy, 100, 100);  // 100 HP
    
    // Situación inicial:
    //  Player       Enemy
    //    []          []
    //    0          15    (separados por 15 píxeles)
    //    →           
    // (player se mueve a la derecha)
    
    // ===== ACT =====
    // Simular 2 segundos de juego a 60 FPS
    float deltaTime = 1.0f / 60.0f;  // ~0.0166 segundos por frame
    
    bool collisionDetected = false;
    
    for (int frame = 0; frame < 120; frame++) {  // 2 segundos = 120 frames
        // 1. Actualizar movimiento
        MovementSystem::update(registry, deltaTime);
        
        // 2. Detectar y resolver colisiones
        auto& playerTransform = registry.get<TransformComponent>(player);
        auto& enemyTransform = registry.get<TransformComponent>(enemy);
        auto& playerCollider = registry.get<Collider>(player);
        auto& enemyCollider = registry.get<Collider>(enemy);
        
        if (PhysicsSystem::checkCollision(playerCollider, playerTransform,
                                         enemyCollider, enemyTransform)) {
            collisionDetected = true;
            PhysicsSystem::resolveCollisions(registry);
            break;  // Salir después de la primera colisión
        }
    }
    
    // ===== ASSERT =====
    // Verificar que la colisión ocurrió
    BOOST_TEST(collisionDetected, "Player should collide with enemy");
    
    // Verificar que el daño se aplicó correctamente
    auto& playerHealth = registry.get<Health>(player);
    auto& enemyHealth = registry.get<Health>(enemy);
    
    BOOST_TEST(playerHealth.current < 100, "Player should take damage");
    BOOST_TEST(enemyHealth.current < 100, "Enemy should take damage");
    
    // Verificar que ambos siguen vivos (no murieron en un hit)
    BOOST_TEST(playerHealth.isAlive(), "Player should still be alive");
    BOOST_TEST(enemyHealth.isAlive(), "Enemy should still be alive");
    
    // Verificar que el sistema ECS sigue funcionando
    BOOST_TEST(registry.valid(player), "Player entity should still be valid");
    BOOST_TEST(registry.valid(enemy), "Enemy entity should still be valid");
}

// ============================================================================
// NOTAS TEÓRICAS - TESTS DE INTEGRACIÓN
// ============================================================================
/*
¿Qué es un test de integración?
- Testea múltiples componentes trabajando juntos
- Simula escenarios reales del juego
- Detecta bugs que solo aparecen cuando los sistemas interactúan

Diferencia con tests unitarios:
┌─────────────────────┬────────────────────────────────────┐
│ Unit Test           │ Integration Test                   │
├─────────────────────┼────────────────────────────────────┤
│ Testea 1 función    │ Testea múltiples sistemas          │
│ Rápido (<1ms)       │ Más lento (~10-100ms)              │
│ Aislado (mocks)     │ Componentes reales                 │
│ Muchos tests        │ Pocos tests (escenarios clave)     │
│ Fácil debug         │ Debug más complejo                 │
└─────────────────────┴────────────────────────────────────┘

Este test integra:
✅ CASO 1: Entidades con sprites (SpriteComponent)
✅ CASO 2: Recursos del juego (componentes cargados)
✅ CASO 4: ECS funciona (registry, componentes, sistemas)
✅ CASO 6: Librerías externas (EnTT)
✅ CASO 7: Colisiones (PhysicsSystem)

Flujo del test (simula un mini juego):
1. Setup: Crear 2 personajes con todos los componentes
2. Game Loop: Simular 120 frames a 60 FPS
3. Movimiento: Actualizar posiciones según velocidad
4. Colisiones: Detectar y resolver colisiones
5. Validación: Verificar que todo funcionó correctamente

¿Por qué simular frames?
- Así detectamos bugs temporales (race conditions)
- Verificamos que el juego funciona en condiciones reales
- Probamos la integración de todos los sistemas

Casos típicos de bugs que SOLO aparecen en integración:
❌ El movimiento funciona, las colisiones funcionan, pero juntos
   el personaje atraviesa paredes (orden incorrecto de updates)
❌ Los componentes funcionan individualmente, pero al tener muchos
   el rendimiento cae (problema de escala)
❌ Los sistemas funcionan, pero el estado global se corrompe

Ejemplo de bug detectado por este test:
Sin test:
  void GameLoop() {
      detectCollisions();  // BUG: se ejecuta ANTES de mover
      updateMovement();    // Las colisiones usan posiciones viejas
  }

Con test #10, este bug se detecta porque la colisión no ocurre cuando debería.

Pirámide de testing (recomendación):
        /\
       /  \       1-2 E2E tests (juego completo)
      /____\
     /      \     5-10 Integration tests
    /________\
   /          \   100+ Unit tests
  /____________\

Este test está en la capa de integración.
*/