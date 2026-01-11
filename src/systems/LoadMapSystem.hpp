#pragma once
#include "../entt/entt.hpp"
#include <string>


struct ObjectProperty {
    std::string name;
    std::string value;
    std::string type;
};

struct MapObject {
    std::string name;
    std::string type;
    float x = 0, y = 0, width = 0, height = 0;
    std::vector<ObjectProperty> properties;
};


void loadTiledMap(const std::string& filename, entt::registry& registry, Texture2D spikeTex, Texture2D wheelTex, Vector2& p1Pos, Vector2& p2Pos);