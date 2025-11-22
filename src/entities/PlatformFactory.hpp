#pragma once
#include "../entt/entt.hpp"
#include <raylib.h>
#include "../components/Trap.hpp"

using namespace std;

entt::entity createPlatform(entt::registry& registry, float x, float y, float width, float height, float vx, float vy, Color color);
