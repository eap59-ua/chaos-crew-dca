#pragma once
#include "../entt/entt.hpp"
#include <string>

struct TileLayer {
    std::string name;
    int width = 0;
    int height = 0;
    std::vector<int> tiles;
};

struct ObjectProperty {
    std::string name;
    std::string value;
};

struct MapObject {
    std::string name;
    std::string type;
    float x = 0, y = 0, width = 0, height = 0;
    std::vector<ObjectProperty> properties;
};

struct MapData {
    int width = 0, height = 0;
    int tileWidth = 0, tileHeight = 0;
    std::vector<TileLayer> layers;
    std::vector<MapObject> objects;
};

MapData loadTiledMap(const std::string& filename);
void createPlatformsFromMap(entt::registry& registry, const MapData& mapData);