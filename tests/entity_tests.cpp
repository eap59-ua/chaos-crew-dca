#define BOOST_TEST_MODULE EntityTests_ChaosCrew
#include <boost/test/included/unit_test.hpp>
#include "../src/entt/entt.hpp"
#include "../src/components/Position.hpp"
#include "../src/components/Velocity.hpp"
#include "../src/components/Sprite.hpp"
#include "../src/components/Player.hpp"
#include "../src/components/Solid.hpp"
#include "../src/components/Platform.hpp"
#include "../src/components/Obstacle.hpp"

// ============================================================================
// TEST #1: Crear entidad con Position y Sprite (CASO 1: Entidades con sprite)
// ============================================================================
// ¿Qué testea? Que se puede crear una entidad con los componentes reales del juego

BOOST_AUTO_TEST_CASE(test_create_entity_with_position_and_sprite) {
    // ===== ARRANGE =====
    entt::registry registry;
    auto entity = registry.create();
    
    // ===== ACT =====
    // Añadir componente Position
    auto& position = registry.emplace<Position>(entity);
    position.x = 100.0f;
    position.y = 200.0f;
    
    // Añadir componente Sprite (como en el juego real)
    auto& sprite = registry.emplace<Sprite>(entity);
    sprite.frameWidth = 32.0f;
    sprite.frameHeight = 32.0f;
    sprite.scale = 2.0f;
    sprite.flipX = false;
    sprite.totalFrames = 11;
    sprite.currentFrame = 0;
    sprite.frameTime = 0.05f;
    
    // ===== ASSERT =====
    BOOST_TEST(registry.all_of<Position>(entity), 
               "Entity should have Position component");
    BOOST_TEST(registry.all_of<Sprite>(entity),
               "Entity should have Sprite component");
    
    // Verificar datos del Position
    auto& retrievedPos = registry.get<Position>(entity);
    BOOST_TEST(retrievedPos.x == 100.0f);
    BOOST_TEST(retrievedPos.y == 200.0f);
    
    // Verificar datos del Sprite
    auto& retrievedSprite = registry.get<Sprite>(entity);
    BOOST_TEST(retrievedSprite.frameWidth == 32.0f);
    BOOST_TEST(retrievedSprite.scale == 2.0f);
    BOOST_TEST(retrievedSprite.totalFrames == 11);
}

// ============================================================================
// TEST #2: Crear jugador completo (CASO 4: ECS funciona)
// ============================================================================
// ¿Qué testea? Que un jugador puede tener todos sus componentes
// ¿Por qué? En Chaos Crew, los jugadores necesitan múltiples componentes

BOOST_AUTO_TEST_CASE(test_create_complete_player_entity) {
    // ===== ARRANGE =====
    entt::registry registry;
    auto playerEntity = registry.create();
    
    // ===== ACT =====
    // Añadir componentes como en PlayerFactory.cpp
    registry.emplace<Position>(playerEntity, 100.0f, 500.0f);
    registry.emplace<Velocity>(playerEntity, 0.0f, 0.0f);
    
    auto& solid = registry.emplace<Solid>(playerEntity);
    solid.width = 32.0f;
    solid.height = 32.0f;
    
    auto& player = registry.emplace<Player>(playerEntity);
    player.isDead = false;
    player.onGround = false;
    player.left = false;
    player.right = false;
    player.jump = false;
    
    auto& sprite = registry.emplace<Sprite>(playerEntity);
    sprite.frameWidth = 32.0f;
    sprite.frameHeight = 32.0f;
    sprite.scale = 2.0f;
    
    // ===== ASSERT =====
    // Verificar que tiene TODOS los componentes necesarios
    BOOST_TEST(registry.all_of<Position>(playerEntity));
    BOOST_TEST(registry.all_of<Velocity>(playerEntity));
    BOOST_TEST(registry.all_of<Solid>(playerEntity));
    BOOST_TEST(registry.all_of<Player>(playerEntity));
    BOOST_TEST(registry.all_of<Sprite>(playerEntity));
    
    // Verificar estado inicial correcto
    auto& playerComp = registry.get<Player>(playerEntity);
    BOOST_TEST(!playerComp.isDead, "Player should start alive");
    BOOST_TEST(!playerComp.onGround, "Player should start in air");
}

// ============================================================================
// TEST #3: Aplicar velocidad a posición (CASO 4: Sistemas funcionan)
// ============================================================================

BOOST_AUTO_TEST_CASE(test_apply_velocity_to_position) {
    // ===== ARRANGE =====
    entt::registry registry;
    auto entity = registry.create();
    
    auto& position = registry.emplace<Position>(entity);
    position.x = 0.0f;
    position.y = 0.0f;
    
    auto& velocity = registry.emplace<Velocity>(entity);
    velocity.vx = 200.0f;  // 200 px/s a la derecha
    velocity.vy = -300.0f; // 300 px/s hacia arriba
    
    float deltaTime = 0.1f;  // 100ms
    
    // ===== ACT =====
    // Simular lo que hace MovementSystem (simplificado)
    position.x += velocity.vx * deltaTime;
    position.y += velocity.vy * deltaTime;
    
    // ===== ASSERT =====
    // Verificar posición después del movimiento
    BOOST_CHECK_CLOSE(position.x, 20.0f, 0.001);   // 0 + 200*0.1 = 20
    BOOST_CHECK_CLOSE(position.y, -30.0f, 0.001);  // 0 + (-300)*0.1 = -30
}

// ============================================================================
// TEST #4: Componente Solid (hitbox) (CASO 4: ECS funciona)
// ============================================================================
// ¿Qué testea? Que el componente Solid funciona correctamente
// ¿Por qué? Solid define la hitbox para colisiones
// Teoría: Unit test, valida componente de colisiones

BOOST_AUTO_TEST_CASE(test_solid_component_hitbox) {
    // ===== ARRANGE =====
    entt::registry registry;
    auto entity = registry.create();
    
    registry.emplace<Position>(entity, 50.0f, 100.0f);
    
    // ===== ACT =====
    auto& solid = registry.emplace<Solid>(entity);
    solid.width = 48.0f;   // Hitbox más pequeña que sprite
    solid.height = 48.0f;
    
    // ===== ASSERT =====
    BOOST_TEST(registry.all_of<Solid>(entity));
    
    auto& retrievedSolid = registry.get<Solid>(entity);
    BOOST_TEST(retrievedSolid.width == 48.0f);
    BOOST_TEST(retrievedSolid.height == 48.0f);
    
    // Verificar que Solid y Position pueden coexistir
    BOOST_TEST(registry.all_of<Position, Solid>(entity),
               "Entity should have both Position and Solid");
}

// ============================================================================
// TEST #5: Componente Platform (CASO 4: ECS funciona)
// ============================================================================
// ¿Qué testea? Que las plataformas tienen el componente correcto
// ¿Por qué? Las plataformas son entidades del juego con comportamiento especial
// Teoría: Unit test, valida componente de plataforma

BOOST_AUTO_TEST_CASE(test_platform_component) {
    // ===== ARRANGE =====
    entt::registry registry;
    auto platformEntity = registry.create();
    
    // ===== ACT =====
    // Crear plataforma como en el juego real
    registry.emplace<Position>(platformEntity, 0.0f, 288.0f);  // y=9 tiles * 32px
    
    auto& solid = registry.emplace<Solid>(platformEntity);
    solid.width = 128.0f;  // 4 tiles de ancho
    solid.height = 32.0f;  // 1 tile de alto
    
    registry.emplace<Platform>(platformEntity);
    
    // ===== ASSERT =====
    BOOST_TEST(registry.all_of<Platform>(platformEntity),
               "Platform entity should have Platform component");
    
    BOOST_TEST(registry.all_of<Position, Solid, Platform>(platformEntity),
               "Platform should have Position, Solid and Platform components");
    
    // Verificar dimensiones típicas de plataforma
    auto& platformSolid = registry.get<Solid>(platformEntity);
    BOOST_TEST(platformSolid.width > 0.0f, "Platform width should be positive");
    BOOST_TEST(platformSolid.height > 0.0f, "Platform height should be positive");
}

// ============================================================================
// TEST #6: Componente Obstacle (CASO 4: ECS funciona)
// ============================================================================
// ¿Qué testea? Que los obstáculos (pinchos, trampas) funcionan
// ¿Por qué? Los obstáculos matan al jugador al tocarlos
// Teoría: Unit test, valida componente de obstáculo

BOOST_AUTO_TEST_CASE(test_obstacle_component) {
    // ===== ARRANGE =====
    entt::registry registry;
    auto spikeEntity = registry.create();
    
    // ===== ACT =====
    // Crear pincho como en el juego real
    registry.emplace<Position>(spikeEntity, 400.0f, 294.4f);  // y=9.2 tiles * 32px
    
    auto& solid = registry.emplace<Solid>(spikeEntity);
    solid.width = 25.6f;   // 0.8 tiles * 32px
    solid.height = 25.6f;
    
    registry.emplace<Obstacle>(spikeEntity);
    
    // ===== ASSERT =====
    BOOST_TEST(registry.all_of<Obstacle>(spikeEntity),
               "Spike entity should have Obstacle component");
    
    BOOST_TEST(registry.all_of<Position, Solid, Obstacle>(spikeEntity),
               "Obstacle should have Position, Solid and Obstacle components");
    
    // Verificar que el obstáculo tiene hitbox pequeña
    auto& obstacleSolid = registry.get<Solid>(spikeEntity);
    BOOST_TEST(obstacleSolid.width < 32.0f, "Spike hitbox should be smaller than 1 tile");
    BOOST_TEST(obstacleSolid.height < 32.0f);
}

// ============================================================================
// TEST #7: Estados del componente Player (CASO 4: ECS funciona)
// ============================================================================
// ¿Qué testea? Que el componente Player maneja estados correctamente
// ¿Por qué? El estado del jugador determina comportamiento (muerto, en suelo, etc.)
// Teoría: Unit test, valida lógica de estado del jugador

BOOST_AUTO_TEST_CASE(test_player_component_states) {
    // ===== ARRANGE =====
    entt::registry registry;
    auto playerEntity = registry.create();
    
    registry.emplace<Position>(playerEntity, 100.0f, 200.0f);
    registry.emplace<Velocity>(playerEntity, 0.0f, 0.0f);
    registry.emplace<Solid>(playerEntity, 32.0f, 32.0f);
    
    // ===== ACT =====
    auto& player = registry.emplace<Player>(playerEntity);
    
    // Estado inicial
    player.isDead = false;
    player.onGround = false;
    player.left = false;
    player.right = false;
    player.jump = false;
    
    // ===== ASSERT =====
    // Verificar estado inicial
    BOOST_TEST(!player.isDead, "Player should start alive");
    BOOST_TEST(!player.onGround, "Player should start in air");
    BOOST_TEST(!player.left && !player.right, "Player should not be moving initially");
    BOOST_TEST(!player.jump, "Player should not be jumping initially");
    
    // Simular que el jugador aterriza
    player.onGround = true;
    BOOST_TEST(player.onGround, "Player should be on ground after landing");
    
    // Simular input de movimiento
    player.right = true;
    BOOST_TEST(player.right, "Player should be moving right");
    
    // Simular muerte
    player.isDead = true;
    BOOST_TEST(player.isDead, "Player should be dead");
}

// ============================================================================
// TEST #8: Gestión de múltiples entidades (CASO 4: ECS funciona)
// ============================================================================
// ¿Qué testea? Que el registry puede manejar muchas entidades a la vez
// ¿Por qué? Un nivel tiene jugadores, enemigos, plataformas, obstáculos
// Teoría: Unit test, valida escalabilidad del ECS

BOOST_AUTO_TEST_CASE(test_multiple_entities_management) {
    // ===== ARRANGE =====
    entt::registry registry;
    
    // ===== ACT =====
    // Crear 2 jugadores
    auto player1 = registry.create();
    registry.emplace<Position>(player1, 0.0f, 0.0f);
    registry.emplace<Player>(player1);
    
    auto player2 = registry.create();
    registry.emplace<Position>(player2, 100.0f, 0.0f);
    registry.emplace<Player>(player2);
    
    // Crear 3 plataformas
    auto platform1 = registry.create();
    registry.emplace<Position>(platform1, 0.0f, 288.0f);
    registry.emplace<Solid>(platform1, 128.0f, 32.0f);
    registry.emplace<Platform>(platform1);
    
    auto platform2 = registry.create();
    registry.emplace<Position>(platform2, 192.0f, 288.0f);
    registry.emplace<Solid>(platform2, 96.0f, 32.0f);
    registry.emplace<Platform>(platform2);
    
    auto platform3 = registry.create();
    registry.emplace<Position>(platform3, 320.0f, 288.0f);
    registry.emplace<Solid>(platform3, 128.0f, 32.0f);
    registry.emplace<Platform>(platform3);
    
    // Crear 2 obstáculos
    auto spike1 = registry.create();
    registry.emplace<Position>(spike1, 150.0f, 300.0f);
    registry.emplace<Solid>(spike1, 25.6f, 25.6f);
    registry.emplace<Obstacle>(spike1);
    
    auto spike2 = registry.create();
    registry.emplace<Position>(spike2, 250.0f, 300.0f);
    registry.emplace<Solid>(spike2, 25.6f, 25.6f);
    registry.emplace<Obstacle>(spike2);
    
    // ===== ASSERT =====
    // Verificar cantidad de entidades
    auto playerView = registry.view<Player>();
    auto platformView = registry.view<Platform>();
    auto obstacleView = registry.view<Obstacle>();
    
    size_t playerCount = 0;
    for (auto entity : playerView) { playerCount++; }
    BOOST_TEST(playerCount == 2, "Should have 2 players");
    
    size_t platformCount = 0;
    for (auto entity : platformView) { platformCount++; }
    BOOST_TEST(platformCount == 3, "Should have 3 platforms");
    
    size_t obstacleCount = 0;
    for (auto entity : obstacleView) { obstacleCount++; }
    BOOST_TEST(obstacleCount == 2, "Should have 2 obstacles");
    
    // Verificar que cada entidad es válida
    BOOST_TEST(registry.valid(player1));
    BOOST_TEST(registry.valid(player2));
    BOOST_TEST(registry.valid(platform1));
    BOOST_TEST(registry.valid(platform2));
    BOOST_TEST(registry.valid(platform3));
    BOOST_TEST(registry.valid(spike1));
    BOOST_TEST(registry.valid(spike2));
}

// ============================================================================
// TEST #9: Eliminar componente (CASO 4: ECS funciona)
// ============================================================================
// ¿Qué testea? Que se pueden eliminar componentes de entidades
// ¿Por qué? A veces necesitas quitar componentes (ej: quitar Velocity al morir)
// Teoría: Unit test, valida gestión dinámica de componentes

BOOST_AUTO_TEST_CASE(test_remove_component) {
    // ===== ARRANGE =====
    entt::registry registry;
    auto entity = registry.create();
    
    registry.emplace<Position>(entity, 50.0f, 50.0f);
    registry.emplace<Velocity>(entity, 100.0f, 0.0f);
    registry.emplace<Player>(entity);
    
    // Verificar que tiene todos los componentes
    BOOST_REQUIRE(registry.all_of<Position, Velocity, Player>(entity));
    
    // ===== ACT =====
    // Eliminar Velocity (simular muerte o congelamiento)
    registry.remove<Velocity>(entity);
    
    // ===== ASSERT =====
    // Verificar que Velocity se eliminó
    BOOST_TEST(!registry.all_of<Velocity>(entity),
               "Velocity component should be removed");
    
    // Verificar que los otros componentes siguen ahí
    BOOST_TEST(registry.all_of<Position>(entity),
               "Position component should still exist");
    BOOST_TEST(registry.all_of<Player>(entity),
               "Player component should still exist");
    
    // Verificar que la entidad sigue siendo válida
    BOOST_TEST(registry.valid(entity),
               "Entity should still be valid after removing component");
}

// ============================================================================
// TEST #10: Filtrado con view (CASO 4: ECS funciona)
// ============================================================================
// ¿Qué testea? Que view<> filtra entidades correctamente
// ¿Por qué? Los sistemas procesan solo entidades con componentes específicos
// Teoría: Unit test, valida sistema de queries del ECS

BOOST_AUTO_TEST_CASE(test_entity_view_filtering) {
    // ===== ARRANGE =====
    entt::registry registry;
    
    // Crear entidades con diferentes combinaciones de componentes
    
    // Entidad 1: Position + Velocity (proyectil)
    auto projectile = registry.create();
    registry.emplace<Position>(projectile, 0.0f, 0.0f);
    registry.emplace<Velocity>(projectile, 500.0f, 0.0f);
    
    // Entidad 2: Position + Velocity + Player (jugador)
    auto player = registry.create();
    registry.emplace<Position>(player, 100.0f, 200.0f);
    registry.emplace<Velocity>(player, 0.0f, 0.0f);
    registry.emplace<Player>(player);
    
    // Entidad 3: Position + Platform (plataforma estática)
    auto platform = registry.create();
    registry.emplace<Position>(platform, 0.0f, 300.0f);
    registry.emplace<Platform>(platform);
    
    // Entidad 4: Position + Velocity + Solid (enemigo en movimiento)
    auto enemy = registry.create();
    registry.emplace<Position>(enemy, 200.0f, 200.0f);
    registry.emplace<Velocity>(enemy, -50.0f, 0.0f);
    registry.emplace<Solid>(enemy, 32.0f, 32.0f);
    
    // ===== ACT & ASSERT =====
    
    // View 1: Todas las entidades con Position + Velocity
    // (MovementSystem usaría esto)
    auto movingView = registry.view<Position, Velocity>();
    size_t movingCount = 0;
    for (auto entity : movingView) { movingCount++; }
    BOOST_TEST(movingCount == 3, "Should have 3 moving entities (projectile, player, enemy)");
    
    // View 2: Solo jugadores (Position + Player)
    auto playerView = registry.view<Position, Player>();
    size_t playerCount = 0;
    for (auto entity : playerView) { playerCount++; }
    BOOST_TEST(playerCount == 1, "Should have 1 player");
    
    // View 3: Entidades con colisión (Position + Solid)
    auto solidView = registry.view<Position, Solid>();
    size_t solidCount = 0;
    for (auto entity : solidView) { solidCount++; }
    BOOST_TEST(solidCount == 1, "Should have 1 solid entity (enemy)");
    
    // View 4: Todas las entidades con Position (todos menos vacíos)
    auto allPositionView = registry.view<Position>();
    size_t allCount = 0;
    for (auto entity : allPositionView) { allCount++; }
    BOOST_TEST(allCount == 4, "Should have 4 entities with Position");
    
    // Verificar que view NO incluye entidades sin los componentes requeridos
    bool platformInMovingView = false;
    for (auto entity : movingView) {
        if (entity == platform) {
            platformInMovingView = true;
        }
    }
    BOOST_TEST(!platformInMovingView, 
               "Platform (no Velocity) should NOT be in moving view");
}

