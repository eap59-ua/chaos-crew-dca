#pragma once
#include "../entt/entt.hpp"
#include <raylib.h>

entt::entity createSpike(entt::registry& registry,
                         float x, float y, float width, float height, Color color);
