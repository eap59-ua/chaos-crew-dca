#include <raylib.h>
#include <iostream>
#include "../entt/entt.hpp"
#include <cmath>
#include <cfloat>
#include "../components/Trap.hpp"
#include "../components/Position.hpp"
#include "../components/Velocity.hpp"
#include "../components/Player.hpp"

using namespace std;

void TrapSystem(entt::registry& registry, float dt)
{
    // =====================================================
    // 1) OBTENER POSICIONES de TODOS LOS JUGADORES
    // =====================================================
    std::vector<Vector2> playerPositions;

    auto players = registry.view<Player, Position>();
    for (auto player : players)
    {
        auto& pos = players.get<Position>(player);
        playerPositions.push_back({ pos.x, pos.y });
    }

    if (playerPositions.empty()) return;  // No hay jugadores → salir


    // =====================================================
    // 2) PROCESAR TODAS LAS TRAMPAS
    // =====================================================
    auto traps = registry.view<Trap, Position>();

    for (auto entity : traps)
    {
        auto& trapPos = traps.get<Position>(entity);
        auto& trap = traps.get<Trap>(entity);


        // =====================================================
        // A) PROXIMITY CONDITION
        // =====================================================
        if (registry.any_of<ProximityCondition>(entity))
        {
            auto& cond = registry.get<ProximityCondition>(entity);

            float minDist = FLT_MAX;

            // revisar distancia con cada jugador
            for (auto& p : playerPositions)
            {
                float dx = trapPos.x - p.x;
                float dy = trapPos.y - p.y;
                float dist = sqrtf(dx * dx + dy * dy);

                if (dist < minDist)
                    minDist = dist;
            }

            trap.conditionMet = (minDist <= cond.distance);
        }


        // =====================================================
        // B) TIMER CONDITION
        // =====================================================
        if (registry.any_of<TimerCondition>(entity))
        {
            auto& cond = registry.get<TimerCondition>(entity);
            cond.elapsed += dt;

            trap.conditionMet = (cond.elapsed >= cond.delay);
        }


        // =====================================================
        // C) PRESSURE PLATE CONDITION
        // =====================================================
        if (registry.any_of<PressurePlateCondition>(entity))
        {
            auto& cond = registry.get<PressurePlateCondition>(entity);
            trap.conditionMet = cond.pressed;
        }


        // =====================================================
        // Si NO se cumple ninguna condición → saltar acciones
        // =====================================================
        if (!trap.conditionMet)
            continue;


        // =====================================================
        // D) MOVE ACTION
        // =====================================================
        if (registry.any_of<MoveAction>(entity))
        {
            auto& mov = registry.get<MoveAction>(entity);

            trapPos.x += mov.dx;
            trapPos.y += mov.dy;
        }


        // =====================================================
        // E) CHANGE VELOCITY ACTION
        // =====================================================
        if (registry.any_of<ChangeVelocityAction>(entity))
        {
            auto& vel = registry.get<Velocity>(entity);
            auto& act = registry.get<ChangeVelocityAction>(entity);

            vel.vx = act.newVelocity;
        }


        // =====================================================
        // F) SPAWN ACTION
        // =====================================================
        if (registry.any_of<SpawnAction>(entity))
        {
            auto& sp = registry.get<SpawnAction>(entity);
        }
        
        registry.remove<Trap>(entity);
    }
}
