#pragma once
#include "../entt/entt.hpp"
#include <raylib.h>

entt::entity createWheel(entt::registry& registry,
                         float x, float y, float radius, Texture2D texture);
