#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <tinyxml2.h>
#include <raylib.h>
#include "../entt/entt.hpp"

#include "../entities/PlatformFactory.hpp"
#include "../entities/DoorFactory.hpp"

#include "../components/Trap.hpp"
#include "../components/Velocity.hpp"

#include "LoadMapSystem.hpp"

using namespace tinyxml2;
using namespace std;

const float TILE_SIZE = 64.0f;  // Tamaño original del tile
const int SCREEN_WIDTH = 1280;   // Resolución de pantalla
const int SCREEN_HEIGHT = 720;

const int pixel_horizontal = 30;
const int pixel_vertical = 10;

// ==== Cargar el mapa XML ====
void loadTiledMap(const std::string& filename, entt::registry& registry) {
    XMLDocument doc;

    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "❌ Error cargando archivo: " << filename << std::endl;
    }

    XMLElement* map = doc.FirstChildElement("map");
    if (!map) {
        std::cerr << "❌ No se encontró el elemento <map>" << std::endl;
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

            // Propiedades personalizadas
            XMLElement* properties = obj->FirstChildElement("properties");
            if (properties) {
            	entt::entity entity = entt::null;

                if(o.type == "Platform") entity = createPlatform(registry, o.x, o.y, o.width, o.height, 0.0f, 0.0f, DARKGRAY);
            	registry.emplace<Trap>(entity, false, false, false);
            	
            	std::string conditionType;
				float conditionValue = 0.f;
				
				std::string actionType;
				float actionValue = 0.f;
				
				float speed = 0.f;
            	
                for (XMLElement* prop = properties->FirstChildElement("property");
                     prop; prop = prop->NextSiblingElement("property"))
                {
                    std::string name = prop->Attribute("name");
    				std::string value = prop->Attribute("value") ? prop->Attribute("value") : "";
				
    				if (name == "condition")            conditionType = value;
    				else if (name == "condition_distance") conditionValue = stof(value);
				
    				else if (name == "action")          actionType = value;
    				else if (name == "action_amount")   actionValue = stof(value);
				
    				else if (name == "speed")           speed = stof(value);
                }
                
                if (registry.any_of<Velocity>(entity))
    				registry.get<Velocity>(entity).vx = speed;
				
				// Añadir condición
				if (conditionType == "proximity")
    				registry.emplace<ProximityCondition>(entity, conditionValue);
				
				// Añadir acción
				if (actionType == "move_horizontal")
    				registry.emplace<MoveAction>(entity, actionValue, 0.f);
				else if (actionType == "move_vertical")
    				registry.emplace<MoveAction>(entity, 0.f, actionValue);
            }
            else {
                if(o.type == "Platform") createPlatform(registry, o.x, o.y, o.width, o.height, 0.0f, 0.0f, DARKGRAY);
                else if(o.type == "Door") createDoor(registry, o.x, o.y, o.width, o.height, GREEN);
            }
        }
    }
}

