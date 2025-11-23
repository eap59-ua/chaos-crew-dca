#pragma once
#include "../entt/entt.hpp"
#include <raylib.h>

void MovementSystem(entt::registry& registry, float deltaTime, int SCREEN_WIDTH, int SCREEN_HEIGHT, Sound jumpSound);