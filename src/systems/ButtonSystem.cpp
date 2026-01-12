#include "ButtonSystem.hpp"
#include "../components/Button.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Player.hpp"
#include <map>
#include <vector>
#include <iostream>

void ButtonSystem(entt::registry& registry) {
    auto viewButtons = registry.view<Button, Position, Solid>();
    auto viewPlayers = registry.view<Player, Position, Solid>();

    // Usaremos esto para agrupar los botones por su canal (ID)
    std::map<int, std::vector<entt::entity>> channelGroups;

    // ---------------------------------------------------------
    // FASE 1: DETECCIÓN INDIVIDUAL Y VISUALES
    // ---------------------------------------------------------
    for (auto entity : viewButtons) {
        auto& btn = viewButtons.get<Button>(entity);
        auto& pos = viewButtons.get<Position>(entity);
        auto& solid = viewButtons.get<Solid>(entity);

        // 1. Agrupar el botón en su canal correspondiente
        channelGroups[btn.channel].push_back(entity);

        // Si ya se ejecutó la acción (ej: puerta abierta), no calculamos más colisiones
        // pero mantenemos el aspecto visual de "pulsado".
        if (btn.executed) {
            btn.isPressed = true; 
            // Visualmente aplastado permanentemente
            btn.color = DARKBLUE;
            btn.bounds.height = solid.height; 
            btn.bounds.y = pos.y;
            continue; 
        }

        // 2. Comprobar si ALGUIEN está pisando ESTE botón ahora mismo
        bool currentFramePressed = false;

        // Sensor un poco más arriba del botón para notar los pies del jugador
        Rectangle sensorRect = { pos.x, pos.y - 4.0f, solid.width, solid.height };

        for (auto pEntity : viewPlayers) {
            auto& pPos = viewPlayers.get<Position>(pEntity);
            auto& pSolid = viewPlayers.get<Solid>(pEntity);
            Rectangle pRect = { pPos.x, pPos.y, pSolid.width, pSolid.height };

            if (CheckCollisionRecs(sensorRect, pRect)) {
                currentFramePressed = true;
                break; // Con que uno lo pise nos vale
            }
        }

        // 3. Actualizar estado y visuales
        btn.isPressed = currentFramePressed;

        if (btn.isPressed) {
            // -- Aspecto PISADO --
            btn.color = RED; // Color indicativo
            btn.bounds.height = solid.height; // Aplastado (altura de hitbox)
            btn.bounds.y = pos.y;             // Pegado al suelo
        } else {
            // -- Aspecto SIN PISAR --
            btn.color = BLUE; // Color normal
            btn.bounds.height = 32.0f;        // Alto visual completo
            btn.bounds.y = pos.y - 22.0f;     // Desplazado hacia arriba
        }

        // Mantener X y ancho sincronizados
        btn.bounds.x = pos.x;
        btn.bounds.width = solid.width;
    }

    // ---------------------------------------------------------
    // FASE 2: LÓGICA DE GRUPO (COOPERATIVO)
    // ---------------------------------------------------------
    // Ahora recorremos los grupos para ver si se cumple la condición TOTAL
    for (auto& [channel, entities] : channelGroups) {
        
        bool allButtonsPressed = true;
        bool alreadyExecuted = false;

        // Revisamos TODOS los botones de este canal
        for (auto entity : entities) {
            auto& btn = registry.get<Button>(entity);
            
            if (!btn.isPressed) {
                allButtonsPressed = false; // ¡Falta uno!
            }
            if (btn.executed) {
                alreadyExecuted = true;
            }
        }

        // LA CONDICIÓN MÁGICA:
        // Todos pisados a la vez Y no se ha ejecutado antes.
        if (allButtonsPressed && !alreadyExecuted) {

            // 1. Ejecutar TODAS las acciones del grupo (cada botón puede tener su propia acción)
            for (auto entity : entities) {
                auto& btn = registry.get<Button>(entity);
                if (btn.onPressAction) {
                    btn.onPressAction(registry);
                    // NO hacemos break aquí - ejecutamos todas las acciones del canal
                }
            }

            // 2. Bloquear el grupo para siempre (se queda resuelto)
            for (auto entity : entities) {
                registry.get<Button>(entity).executed = true;
            }
        }
    }
}