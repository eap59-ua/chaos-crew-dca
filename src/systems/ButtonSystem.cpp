#include "ButtonSystem.hpp"
#include "../components/Button.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Player.hpp"
#include "../components/Sprite.hpp"
#include <map>
#include <vector>

void ButtonSystem(entt::registry& registry) {
    
    // Usamos un mapa de vectores para agrupar las entidades por canal
    std::map<int, std::vector<entt::entity>> channelGroups;

    auto viewButtons = registry.view<Button, Position, Solid, Sprite>();
    auto viewPlayers = registry.view<Player, Position, Solid>();

    // ---------------------------------------------------------
    // FASE 1: Detectar colisiones y agrupar
    // ---------------------------------------------------------
    for (auto entity : viewButtons) {
        auto& btn = viewButtons.get<Button>(entity);
        auto& pos = viewButtons.get<Position>(entity);
        auto& solid = viewButtons.get<Solid>(entity);
        
        // Agrupamos
        channelGroups[btn.channel].push_back(entity);

        // Si ya se ejecutó, no necesitamos comprobar colisiones, 
        // el puzle de este botón ya está resuelto.
        if (btn.executed) continue; 

        // Reset momentáneo
        btn.isPressed = false;
        
        Rectangle btnRect = {pos.x, pos.y, solid.width, solid.height};

        // Chequear colisión con jugadores
        for (auto pEntity : viewPlayers) {
            auto& pPos = viewPlayers.get<Position>(pEntity);
            auto& pSolid = viewPlayers.get<Solid>(pEntity);
            Rectangle pRect = {pPos.x, pPos.y, pSolid.width, pSolid.height};

            if (CheckCollisionRecs(btnRect, pRect)) {
                btn.isPressed = true;
                break; 
            }
        }
    }

    // ---------------------------------------------------------
    // FASE 2: Lógica Cooperativa y Ejecución
    // ---------------------------------------------------------
    for (auto& [channel, entities] : channelGroups) {
        
        bool allPressed = true;
        bool alreadyExecuted = false;

        // Comprobamos el estado del grupo
        for (auto e : entities) {
            auto& btn = registry.get<Button>(e);
            if (!btn.isPressed) allPressed = false;
            if (btn.executed) alreadyExecuted = true;
        }

        // CONDICIÓN DE ÉXITO: Todos pisados Y no se ha hecho antes
        if (allPressed && !alreadyExecuted) {
            
            // 1. Ejecutar la función (usamos la del primer botón del grupo, por ejemplo)
            // Asegúrate de que al menos uno tenga función asignada
            if (!entities.empty()) {
                auto& firstBtn = registry.get<Button>(entities[0]);
                if (firstBtn.onPressAction) {
                    firstBtn.onPressAction(registry); // ¡BOOM! Acción ejecutada
                }
            }

            // 2. BLOQUEAR EL ESTADO (Marcar todos como ejecutados)
            for (auto e : entities) {
                registry.get<Button>(e).executed = true;
            }
        }

        // ---------------------------------------------------------
        // FASE 3: Feedback Visual Permanente
        // ---------------------------------------------------------
        for (auto e : entities) {
            auto& btn = registry.get<Button>(e);
            auto& sprite = registry.get<Sprite>(e);

            // Si se está pisando AHORA o si ya se COMPLETÓ el puzle -> Frame hundido
            if (btn.isPressed || btn.executed) {
                sprite.currentFrame = 1; // Botón abajo
            } else {
                sprite.currentFrame = 0; // Botón arriba
            }
        }
    }
}