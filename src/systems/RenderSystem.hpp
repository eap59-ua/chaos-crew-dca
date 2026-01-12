#pragma once
#include "../entt/entt.hpp"

void renderButtons(entt::registry& registry);
void renderPlayers(entt::registry& registry);
void renderPlatforms(entt::registry& registry,Texture2D terrainTex);
void renderDoors(entt::registry& registry,Texture2D doorTex);
void renderScene(entt::registry& registry, Texture2D terrainTex, Texture2D doorTex);