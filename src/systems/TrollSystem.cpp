#include <raylib.h>
#include <iostream>
#include "../entt/entt.hpp"
#include <cmath>
#include <cfloat>
#include "../components/Trap.hpp"
#include "../components/Position.hpp"
#include "../components/Velocity.hpp"
#include "../components/Player.hpp"
#include "../components/Solid.hpp"

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
    auto traps = registry.view<Trap, Position, Solid, Velocity>();

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
            	auto& size = registry.get<Solid>(entity);
            	float closestX = fmaxf(trapPos.x, fminf(p.x, trapPos.x + size.width));
            	float closestY = fmaxf(trapPos.y, fminf(p.y, trapPos.y + size.height));
            
                float dx = p.x - closestX;
                float dy = p.y - closestY;
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
        //  Si la condición se cumple por primera vez → activar acción
        // =====================================================
        if (trap.conditionMet && !trap.actionMet)
        {
            trap.actionMet = true;
        }

        // =====================================================
        //  Si la acción no ha empezado, no hacer nada
        // =====================================================
        if (!trap.actionMet)
            continue;

        // =====================================================
        // D) MOVE ACTION
        // =====================================================
        if (registry.any_of<MoveAction>(entity))
        {
            auto& mov = registry.get<MoveAction>(entity);
            auto& trapVel = traps.get<Velocity>(entity);

            if(mov.dx < 0 && trapVel.vx > 0) trapVel.vx *= -1;
            if(mov.dy < 0 && trapVel.vy > 0) trapVel.vy *= -1;

            trapPos.x += trapVel.vx;
            trapPos.y += trapVel.vy;

            mov.dx -= trapVel.vx;
            mov.dy -= trapVel.vy;

            if(mov.dx == 0 && mov.dy == 0)
            {
                trap.finMet = true;
            }
        }
        
        // =====================================================
        // E) CHANGE DIMENSION ACTION
        // =====================================================
        if (registry.any_of<ChangeDimensionAction>(entity))
        {
            auto& dimension = registry.get<ChangeDimensionAction>(entity);
            auto& size = registry.get<Solid>(entity);
            
            float scale = 10.0;

            if(dimension.dh < 0) {
            	scale *= -1;
            }
            
            //size.height -= scale / 10;
            //trapPos.y += scale / 30;
            size.width -= scale;
            trapPos.x += scale / 2;
            
            dimension.dh -= scale;

            if(dimension.dh == 0)
            {
                trap.finMet = true;
            }
        }


        // =====================================================
        // F) CHANGE VELOCITY ACTION
        // =====================================================
        if (registry.any_of<ChangeVelocityAction>(entity))
        {
            auto& vel = registry.get<Velocity>(entity);
            auto& act = registry.get<ChangeVelocityAction>(entity);

            vel.vx = act.newVelocityX;
            vel.vy = act.newVelocityY;
        }


        // =====================================================
        // G) SPAWN ACTION
        // =====================================================
        if (registry.any_of<SpawnAction>(entity))
        {
            auto& sp = registry.get<SpawnAction>(entity);
        }
        
        if(trap.finMet) registry.remove<Trap>(entity);
    }
}
