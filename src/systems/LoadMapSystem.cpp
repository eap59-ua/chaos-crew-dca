#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <tinyxml2.h>
#include <raylib.h>
#include "../entt/entt.hpp"
#include "../entities/PlatformFactory.hpp"
#include "LoadMapSystem.hpp"

using namespace tinyxml2;
using namespace std;

const float TILE_SIZE = 64.0f;  // Tamaño original del tile
const int SCREEN_WIDTH = 1280;   // Resolución de pantalla
const int SCREEN_HEIGHT = 720;

const int pixel_horizontal = 30;
const int pixel_vertical = 10;

// ==== Cargar el mapa XML ====
MapData loadTiledMap(const std::string& filename) {
    MapData mapData;
    XMLDocument doc;

    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "❌ Error cargando archivo: " << filename << std::endl;
        return mapData;
    }

    XMLElement* map = doc.FirstChildElement("map");
    if (!map) {
        std::cerr << "❌ No se encontró el elemento <map>" << std::endl;
        return mapData;
    }

    /*map->QueryIntAttribute("width", &mapData.width);
    map->QueryIntAttribute("height", &mapData.height);
    map->QueryIntAttribute("tilewidth", &mapData.tileWidth);
    map->QueryIntAttribute("tileheight", &mapData.tileHeight);

    // Calcular el factor de escala según la resolución de la pantalla
    float scaleX = static_cast<float>(SCREEN_WIDTH) / (mapData.width * TILE_SIZE);
    float scaleY = static_cast<float>(SCREEN_HEIGHT) / (mapData.height * TILE_SIZE);
    float scale = std::min(scaleX, scaleY); // Escala uniforme para no distorsionar el mapa*/

    // ===== Leer capas de tiles =====
    for (XMLElement* layer = map->FirstChildElement("layer"); layer; layer = layer->NextSiblingElement("layer")) {
        TileLayer tileLayer;
        tileLayer.name = layer->Attribute("name") ? layer->Attribute("name") : "";
        layer->QueryIntAttribute("width", &tileLayer.width);
        layer->QueryIntAttribute("height", &tileLayer.height);

        XMLElement* data = layer->FirstChildElement("data");
        if (data && data->GetText()) {
            std::stringstream ss(data->GetText());
            std::string value;
            while (std::getline(ss, value, ',')) {
                if (!value.empty())
                    tileLayer.tiles.push_back(std::stoi(value));
            }
        }
        mapData.layers.push_back(tileLayer);
    }

    // ===== Leer grupo de objetos =====
    for (XMLElement* objectGroup = map->FirstChildElement("objectgroup");
         objectGroup; objectGroup = objectGroup->NextSiblingElement("objectgroup"))
    {
        for (XMLElement* obj = objectGroup->FirstChildElement("object");
             obj; obj = obj->NextSiblingElement("object"))
        {
            MapObject o;
            o.name = obj->Attribute("name") ? obj->Attribute("name") : "";
            o.type = obj->Attribute("type") ? obj->Attribute("type") : "";
            obj->QueryFloatAttribute("x", &o.x);
            obj->QueryFloatAttribute("y", &o.y);
            obj->QueryFloatAttribute("width", &o.width);
            obj->QueryFloatAttribute("height", &o.height);

            // Aplicar la constante TILE_SIZE para escalar
            o.x *= SCREEN_WIDTH / pixel_horizontal;
            o.y *= SCREEN_HEIGHT / pixel_vertical;
            o.width *= SCREEN_WIDTH / pixel_horizontal;
            o.height *= SCREEN_HEIGHT / pixel_vertical;

            // Aquí, ajustamos según la escala
            /*
            o.x *= scale;
            o.y *= scale;
            o.width *= scale;
            o.height *= scale;
            */

            // Propiedades personalizadas
            XMLElement* properties = obj->FirstChildElement("properties");
            if (properties) {
                for (XMLElement* prop = properties->FirstChildElement("property");
                     prop; prop = prop->NextSiblingElement("property"))
                {
                    ObjectProperty p;
                    p.name = prop->Attribute("name") ? prop->Attribute("name") : "";
                    p.value = prop->Attribute("value") ? prop->Attribute("value") : "";
                    o.properties.push_back(p);
                }
            }

            mapData.objects.push_back(o);
        }
    }

    return mapData;
}

// ==== Crear plataformas desde el mapa ====
void createPlatformsFromMap(entt::registry& registry, const MapData& mapData) {
    for (const auto& obj : mapData.objects) {
        if (obj.type == "platform" || obj.type == "Platform") {
            Color color = GRAY;

            // Buscar propiedad "color" si existe
            for (const auto& prop : obj.properties) {
                if (prop.name == "color") {
                    if (prop.value == "red") color = RED;
                    else if (prop.value == "green") color = GREEN;
                    else if (prop.value == "blue") color = BLUE;
                }
            }

            // Crear plataforma con las nuevas coordenadas y tamaños ajustados
            createPlatform(registry, obj.x, obj.y, obj.width, obj.height, color);
            std::cout << "🧱 Plataforma creada en (" << obj.x << "," << obj.y - obj.height 
                      << ") tamaño (" << obj.width << "x" << obj.height << ")\n";
        }
    }
}
