#define BOOST_TEST_MODULE CollisionTests
#include <boost/test/included/unit_test.hpp>
#include <algorithm>
#include <cmath>

// ============================================================================
// SISTEMA DE COLISIONES (CASO 7)
// ============================================================================
// implementamos detección de colisiones AABB (Axis-Aligned Bounding Box)


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


class PhysicsSystem {
public:
    // Detectar si dos cajas colisionan
    static bool checkCollision(const AABB& a, const AABB& b) {
        // Implementación directa del algoritmo AABB
        return !(a.right() <= b.left() ||   //A está a la izquierda de B
                 a.left() >= b.right() ||   //A está a la derecha de B
                 a.bottom() <= b.top() ||   //A está arriba de B
                 a.top() >= b.bottom());    //A está abajo de B
    }
    
    // Calcular penetración horizontal
    static float calculateHorizontalPenetration(const AABB& a, const AABB& b) {
        float leftPenetration = a.right() - b.left();
        float rightPenetration = b.right() - a.left();
        
        // Retornar la penetración más pequeña
        if (std::abs(leftPenetration) < std::abs(rightPenetration)) {
            return leftPenetration;
        }
        return -rightPenetration;
    }
};

// ============================================================================
// TEST #8: Detectar colisión entre cajas superpuestas (CASO 7)


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
    BOOST_TEST(std::abs(penetration - 10.0f) < 0.001f, 
               "Penetration should be 10 pixels");
    
    // Caso contrario: B empuja a A desde la derecha
    float penetrationReverse = PhysicsSystem::calculateHorizontalPenetration(boxB, boxA);
    BOOST_TEST(std::abs(penetrationReverse + 10.0f) < 0.001f,
               "Reverse penetration should be -10 pixels");
}

