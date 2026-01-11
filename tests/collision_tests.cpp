#define BOOST_TEST_MODULE CollisionTests
#include <boost/test/included/unit_test.hpp>
#include <algorithm>
#include <cmath>

// ============================================================================
// SISTEMA DE COLISIONES (CASO 7)
// ============================================================================
// Este código implementa detección de colisiones AABB (Axis-Aligned Bounding Box)
// Es fundamental para cualquier juego 2D

struct AABB {
    float x, y;          // Posición (esquina superior izquierda)
    float width, height; // Dimensiones
    
    AABB(float x, float y, float w, float h) 
        : x(x), y(y), width(w), height(h) {}
    
    // Obtener bordes de la caja
    float left() const { return x; }
    float right() const { return x + width; }
    float top() const { return y; }
    float bottom() const { return y + height; }
};

// ============================================================================
// ALGORITMO DE DETECCIÓN AABB
// ============================================================================
/*
Teoría: Dos cajas NO colisionan si:
- A está completamente a la izquierda de B, O
- A está completamente a la derecha de B, O
- A está completamente arriba de B, O
- A está completamente abajo de B

Si ninguna de estas condiciones se cumple → HAY COLISIÓN

Fórmula matemática:
NO_COLISION = (A.right < B.left) || (A.left > B.right) ||
              (A.bottom < B.top) || (A.top > B.bottom)

COLISION = !NO_COLISION
*/

class PhysicsSystem {
public:
    // Detectar si dos cajas colisionan
    static bool checkCollision(const AABB& a, const AABB& b) {
        // Implementación directa del algoritmo AABB
        return !(a.right() <= b.left() ||   // A está a la izquierda de B
                 a.left() >= b.right() ||   // A está a la derecha de B
                 a.bottom() <= b.top() ||   // A está arriba de B
                 a.top() >= b.bottom());    // A está abajo de B
    }
    
    // Calcular penetración horizontal (para resolver colisiones)
    static float calculateHorizontalPenetration(const AABB& a, const AABB& b) {
        float leftPenetration = a.right() - b.left();
        float rightPenetration = b.right() - a.left();
        
        // Retornar la penetración más pequeña (resolver por el camino más corto)
        if (std::abs(leftPenetration) < std::abs(rightPenetration)) {
            return leftPenetration;
        }
        return -rightPenetration;
    }
};

// ============================================================================
// TEST #8: Detectar colisión entre cajas superpuestas (CASO 7)
// ============================================================================
// ¿Qué testea? Que dos cajas que se solapan son detectadas como colisión
// ¿Por qué? Es la base de la física del juego (personaje vs enemigo, etc.)
// Teoría: Unit test, BlackBox (solo verificamos resultado, no algoritmo)

BOOST_AUTO_TEST_CASE(test_detect_overlapping_boxes) {
    // ===== ARRANGE =====
    // Crear dos cajas que se solapan
    //
    //     Caja A (50x50 desde 0,0)
    //     +--------+
    //     |        |
    //     |   +----+----+  Caja B (50x50 desde 25,25)
    //     |   |####|    |  (área solapada: 25x25)
    //     +---|----+    |
    //         |         |
    //         +---------+
    //
    AABB boxA(0.0f, 0.0f, 50.0f, 50.0f);      // Posición: (0, 0)
    AABB boxB(25.0f, 25.0f, 50.0f, 50.0f);    // Posición: (25, 25)
    
    // ===== ACT =====
    bool collision = PhysicsSystem::checkCollision(boxA, boxB);
    
    // ===== ASSERT =====
    BOOST_TEST(collision, "Overlapping boxes should collide");
    
    // Verificar simetría (A vs B = B vs A)
    bool collisionReverse = PhysicsSystem::checkCollision(boxB, boxA);
    BOOST_TEST(collisionReverse, "Collision should be symmetric");
}

// ============================================================================
// TEST #9: Calcular penetración horizontal (CASO 7)
// ============================================================================
// ¿Qué testea? Que se calcula correctamente cuánto se solapan las cajas
// ¿Por qué? Para resolver colisiones (empujar al personaje fuera del muro)
// Teoría: Unit test, WhiteBox (conocemos la fórmula interna)

BOOST_AUTO_TEST_CASE(test_calculate_horizontal_penetration) {
    // ===== ARRANGE =====
    // Caja A: posición (0, 0), tamaño 50x50
    // Caja B: posición (40, 0), tamaño 50x50
    //
    // Visualización:
    //     A               B
    //  +------+      +------+
    //  |      |######|      |  ← Solapamiento de 10 píxeles
    //  |      |######|      |
    //  +------+      +------+
    //  0     50     40     90
    //        └──10──┘
    //
    AABB boxA(0.0f, 0.0f, 50.0f, 50.0f);   // right = 50
    AABB boxB(40.0f, 0.0f, 50.0f, 50.0f);  // left = 40
    
    // ===== ACT =====
    float penetration = PhysicsSystem::calculateHorizontalPenetration(boxA, boxB);
    
    // ===== ASSERT =====
    // La penetración es 10 píxeles (50 - 40 = 10)
    BOOST_TEST(std::abs(penetration - 10.0f) < 0.001f, 
               "Penetration should be 10 pixels");
    
    // Caso contrario: B empuja a A desde la derecha
    float penetrationReverse = PhysicsSystem::calculateHorizontalPenetration(boxB, boxA);
    BOOST_TEST(std::abs(penetrationReverse + 10.0f) < 0.001f,
               "Reverse penetration should be -10 pixels");
}

// ============================================================================
// NOTAS TEÓRICAS - FÍSICA DE COLISIONES
// ============================================================================
/*
¿Por qué AABB (Axis-Aligned Bounding Box)?
✅ Muy rápido (solo comparaciones de floats)
✅ Suficiente para la mayoría de juegos 2D
✅ Fácil de implementar y testear

Limitaciones:
❌ No funciona para objetos rotados
❌ No es perfecto para formas irregulares
→ Solución: Usar múltiples AABBs o círculos

Fases de la detección de colisiones:
1. Broad Phase: Descartar objetos lejanos (QuadTree, Grid)
2. Narrow Phase: Detección precisa (AABB que testeamos aquí)
3. Resolución: Separar objetos (usando penetración)

Ejemplo de resolución en el juego:
void GameSystem::resolveCollision(Entity player, Entity wall) {
    auto& playerAABB = registry.get<Collider>(player);
    auto& wallAABB = registry.get<Collider>(wall);
    
    float penetration = calculateHorizontalPenetration(playerAABB, wallAABB);
    
    // Mover al jugador fuera del muro
    auto& transform = registry.get<Transform>(player);
    transform.x -= penetration;  // Empujar al jugador
}

Casos especiales a testear:
✅ Colisión normal (Test #8)
✅ Penetración (Test #9)
✅ No colisión (cajas separadas)
✅ Colisión en bordes (edge case)
✅ Cajas del mismo tamaño vs diferentes tamaños

¿Por qué usar tolerancia (0.001f) para floats?
Los floats tienen errores de redondeo:
  0.1 + 0.2 != 0.3  (es 0.30000000000000004)

Por eso NUNCA usar == para floats:
  ❌ if (penetration == 10.0f)
  ✅ if (std::abs(penetration - 10.0f) < 0.001f)

O en Boost.Test:
  ✅ BOOST_CHECK_CLOSE(penetration, 10.0f, 0.01);  // 0.01% tolerancia
*/