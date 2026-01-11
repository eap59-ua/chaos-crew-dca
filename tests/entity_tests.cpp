#define BOOST_TEST_MODULE EntityTests
#include <boost/test/included/unit_test.hpp>
#include <entt/entt.hpp>

// ============================================================================
// COMPONENTES DEL JUEGO (CASO 1: Entidades con sprite)
// ============================================================================
// Estos son los componentes que usará tu juego real

struct SpriteComponent {
    int textureID;    // ID de la textura en Raylib
    float x, y;       // Posición en pantalla
    float width, height;
};

struct TransformComponent {
    float x, y;
    float rotation;
    float scaleX, scaleY;
};

struct VelocityComponent {
    float vx, vy;     // Velocidad en pixels/segundo
};

// ============================================================================
// TEST #1: Crear entidad con sprite (CASO 1)
// ============================================================================
// ¿Qué testea? Que se puede crear una entidad y añadirle un componente Sprite
// ¿Por qué? Es fundamental para mostrar gráficos en el juego
// Teoría: Unit test, BlackBox (solo miramos el resultado)

BOOST_AUTO_TEST_CASE(test_create_entity_with_sprite) {
    // ===== ARRANGE =====
    // Crear el registro ECS (contenedor de entidades)
    entt::registry registry;
    auto entity = registry.create();
    
    // ===== ACT =====
    // Añadir componente Sprite a la entidad
    auto& sprite = registry.emplace<SpriteComponent>(entity);
    sprite.textureID = 42;
    sprite.x = 101.0f;
    sprite.y = 200.0f;
    sprite.width = 32.0f;
    sprite.height = 32.0f;
    
    // ===== ASSERT =====
    // Verificar que el componente existe y tiene los datos correctos
    BOOST_TEST(registry.all_of<SpriteComponent>(entity), 
               "Entity should have SpriteComponent");
    
    auto& retrievedSprite = registry.get<SpriteComponent>(entity);
    BOOST_TEST(retrievedSprite.textureID == 42);
    BOOST_TEST(retrievedSprite.x == 100.0f);
    BOOST_TEST(retrievedSprite.y == 200.0f);
}

// ============================================================================
// TEST #2: Añadir múltiples componentes (CASO 4: ECS funciona)
// ============================================================================
// ¿Qué testea? Que una entidad puede tener múltiples componentes
// ¿Por qué? Los personajes del juego necesitan Transform + Sprite + Velocity
// Teoría: Unit test, verifica arquitectura ECS

BOOST_AUTO_TEST_CASE(test_entity_multiple_components) {
    // ===== ARRANGE =====
    entt::registry registry;
    auto entity = registry.create();
    
    // ===== ACT =====
    // Añadir 3 componentes diferentes
    registry.emplace<TransformComponent>(entity, 50.0f, 75.0f, 0.0f, 1.0f, 1.0f);
    registry.emplace<SpriteComponent>(entity, 1, 50.0f, 75.0f, 32.0f, 32.0f);
    registry.emplace<VelocityComponent>(entity, 5.0f, -3.0f);
    
    // ===== ASSERT =====
    // Verificar que la entidad tiene los 3 componentes
    BOOST_TEST(registry.all_of<TransformComponent>(entity));
    BOOST_TEST(registry.all_of<SpriteComponent>(entity));
    BOOST_TEST(registry.all_of<VelocityComponent>(entity));
}

// ============================================================================
// TEST #3: Sistema de movimiento (CASO 4: Update funciona)
// ============================================================================
// ¿Qué testea? Que el sistema de movimiento actualiza posiciones correctamente
// ¿Por qué? Es la base del gameplay (mover personajes)
// Teoría: Unit test + WhiteBox (conocemos la implementación del sistema)

BOOST_AUTO_TEST_CASE(test_movement_system_update) {
    // ===== ARRANGE =====
    entt::registry registry;
    auto entity = registry.create();
    registry.emplace<TransformComponent>(entity, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    registry.emplace<VelocityComponent>(entity, 10.0f, 5.0f);
    
    float deltaTime = 1.0f;  // 1 segundo de simulación
    
    // ===== ACT =====
    // Simular el sistema de movimiento (esto es lo que hará tu GameSystem)
    auto view = registry.view<TransformComponent, VelocityComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& velocity = view.get<VelocityComponent>(entity);
        
        // Fórmula física: posición += velocidad * tiempo
        transform.x += velocity.vx * deltaTime;
        transform.y += velocity.vy * deltaTime;
    }
    
    // ===== ASSERT =====
    // Verificar que la posición se actualizó correctamente
    auto& transform = registry.get<TransformComponent>(entity);
    BOOST_TEST(transform.x == 10.0f);  // 0 + 10*1 = 10
    BOOST_TEST(transform.y == 5.0f);   // 0 + 5*1 = 5
}

// ============================================================================
// NOTAS TEÓRICAS
// ============================================================================
/*
BOOST_TEST vs BOOST_REQUIRE:
- BOOST_TEST: Si falla, continúa ejecutando el test (para ver múltiples errores)
- BOOST_REQUIRE: Si falla, detiene el test inmediatamente (crítico)

Ejemplo:
BOOST_REQUIRE(registry.valid(entity));  // Crítico: si no es válido, no seguir
BOOST_TEST(transform.x == 10.0f);       // Si falla, ver otros asserts también

BOOST_CHECK_CLOSE para floats:
- NUNCA usar == para comparar floats (errores de redondeo)
- Usar: BOOST_CHECK_CLOSE(actual, expected, tolerance)
- Ejemplo: BOOST_CHECK_CLOSE(transform.x, 10.0f, 0.0001);
*/