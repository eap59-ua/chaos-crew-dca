#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <tinyxml2.h>
#include <raylib.h>
#include "../entt/entt.hpp"

#include "../entities/PlatformFactory.hpp"
#include "../entities/DoorFactory.hpp"
#include "../entities/SpikeFactory.hpp"
#include "../entities/WheelFactory.hpp"

#include "../components/Trap.hpp"
#include "../components/Position.hpp"
#include "../components/Velocity.hpp"
#include "../components/Solid.hpp"
#include "../components/Player.hpp"
#include "../components/Patron.hpp"
#include "../components/Door.hpp"

#include "../entities/ButtonFactory.hpp" // nuevo: idea para hacerlo colaborativo
#include "../components/Button.hpp"      // nuevo: idea para hacerlo colaborativo


#include "LoadMapSystem.hpp"

#include <cmath>
#include <cfloat>

using namespace tinyxml2;
using namespace std;

const float TILE_SIZE = 64.0f;  // Tamaño original del tile
const int SCREEN_WIDTH = 1280;   // Resolución de pantalla
const int SCREEN_HEIGHT = 720;

const int pixel_horizontal = 30;
const int pixel_vertical = 10;

// ------------------------
// Prototipos
// ------------------------

static void ProcessTrap(entt::registry& registry, entt::entity entity,
                       const std::string& conditionType, float conditionValue,
                       const std::string& actionType, float actionValue,
                       float speed);

static void AddProximityCondition(entt::registry& registry, entt::entity e, float distance);
static void AddTimerCondition(entt::registry& registry, entt::entity e, float delay);

static void AddMoveHorizontalAction(entt::registry& registry, entt::entity e, float amount);
static void AddMoveVerticalAction(entt::registry& registry, entt::entity e, float amount);
static void AddVelocityHorizontalAction(entt::registry& registry, entt::entity e, float newVx);
static void AddVelocityVerticalAction(entt::registry& registry, entt::entity e, float newVy);
static void AddChangeDimensionAction(entt::registry& registry, entt::entity e, float amount);
// static void AddSpawnAction(entt::registry& registry, entt::entity e, const std::string& enemyType);


static void AddPatronLoopHorizontal(entt::registry& registry, entt::entity e, float amount);
static void AddPatronLoopVertical(entt::registry& registry, entt::entity e, float amount);

// ==== Cargar el mapa XML ====
void loadTiledMap(const std::string& filename, entt::registry& registry, Texture2D spikeTex, Texture2D wheelTex) {
    XMLDocument doc;

    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "Error cargando archivo: " << filename << std::endl;
        return;
    }

    XMLElement* map = doc.FirstChildElement("map");
    if (!map) {
        std::cerr << "No se encontró el elemento <map>" << std::endl;
        return;
    }

    // ---------------------------------------------------------
    // 1. DEFINIR ESCALAS (Corrección del error scaleX/scaleY)
    // ---------------------------------------------------------
    // Usamos float para no perder precisión en la división
    float scaleX = (float)SCREEN_WIDTH / (float)pixel_horizontal;
    float scaleY = (float)SCREEN_HEIGHT / (float)pixel_vertical;

    for (XMLElement* objectGroup = map->FirstChildElement("objectgroup");
         objectGroup; objectGroup = objectGroup->NextSiblingElement("objectgroup"))
    {
        const string groupName = objectGroup->Attribute("name");

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

            // ---------------------------------------------------------
            // 2. APLICAR ESCALAS USANDO LAS VARIABLES
            // ---------------------------------------------------------
            o.x *= scaleX;
            o.y *= scaleY;
            o.width *= scaleX;
            o.height *= scaleY;

            XMLElement* properties = obj->FirstChildElement("properties");
            
            // --- CAPA DE TRAMPAS ---
            if (properties && groupName == "ObjectsTraps")
            {
                entt::entity entity = entt::null;
                if (o.type == "Platform")
                    entity = createPlatform(registry, o.x, o.y, o.width, o.height, 0.0f, 0.0f, DARKGRAY);
                else if (o.type == "Door")
                    entity = createDoor(registry, o.x, o.y, o.width, o.height, GREEN);
                else if(o.type == "Spike")
                    entity = createSpike(registry, o.x, o.y, o.width, o.height, spikeTex);
                else if(o.type == "Wheel")
                    entity = createWheel(registry, o.x, o.y, o.width / 2.0f, wheelTex);
                
                if (entity == entt::null) {
                     // Si no es un tipo conocido, saltamos o lanzamos error (opcional)
                     continue; 
                }

                auto &trap = registry.get_or_emplace<Trap>(entity);

                // Variables temporales
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

                    if (name == "condition") {
                        if (!conditionType.empty() && !actionType.empty()) {
                            ProcessTrap(registry, entity, conditionType, conditionValue, actionType, actionValue, speed);
                            conditionType.clear(); actionType.clear();
                            conditionValue = 0.f; actionValue = 0.f; speed = 0.f;
                        }
                        conditionType = value;
                    }
                    else if (name == "condition_value") conditionValue = value.empty() ? 0.f : stof(value);
                    else if (name == "action") actionType = value;
                    else if (name == "action_amount") actionValue = value.empty() ? 0.f : stof(value);
                    else if (name == "speed") speed = value.empty() ? 0.f : stof(value);
                }

                if (!conditionType.empty() && !actionType.empty()) {
                    ProcessTrap(registry, entity, conditionType, conditionValue, actionType, actionValue, speed);
                }
            }
            // --- CAPA DE LÓGICA (Plataformas móviles simples) ---
            else if (properties && groupName == "ObjectsLogic")
            {
                entt::entity entity = entt::null;
                if (o.type == "Platform")
                    entity = createPlatform(registry, o.x, o.y, o.width, o.height, 0.0f, 0.0f, DARKGRAY);
                else if (o.type == "Door")
                    entity = createDoor(registry, o.x, o.y, o.width, o.height, GREEN);
                else if(o.type == "Spike")
                    entity = createSpike(registry, o.x, o.y, o.width, o.height, spikeTex);
                else if(o.type == "Wheel")
                    entity = createWheel(registry, o.x, o.y, o.width / 2.0f, wheelTex);

                if (entity == entt::null) continue;

                for (XMLElement* prop = properties->FirstChildElement("property");
                    prop; prop = prop->NextSiblingElement("property"))
                {
                    std::string name = prop->Attribute("name");
                    std::string value = prop->Attribute("value") ? prop->Attribute("value") : "";

                    if (name == "speed") {
                        float v = stof(value);
                        auto &vel = registry.get_or_emplace<Velocity>(entity);
                        vel.vx = v; vel.vy = v;
                    }
                    else if (name == "bucle_horizontal") AddPatronLoopHorizontal(registry, entity, stof(value));
                    else if (name == "bucle_vertical") AddPatronLoopVertical(registry, entity, stof(value));
                }
            }
            // --- CAPA DE BOTONES (Nueva lógica arreglada) ---
            else if (properties && groupName == "ObjectsButtons")
            {
                if (o.type == "Button") {
                    
                    int channel = 0;
                    std::string actionType = "";
                    float spawnX = 0, spawnY = 0, spawnW = 0, spawnH = 0;

                    for (XMLElement* prop = properties->FirstChildElement("property");
                         prop; prop = prop->NextSiblingElement("property"))
                    {
                        std::string name = prop->Attribute("name");
                        std::string value = prop->Attribute("value") ? prop->Attribute("value") : "";

                        if (name == "channel") channel = std::stoi(value);
                        else if (name == "action_type") actionType = value;
                        else if (name == "spawn_x") spawnX = std::stof(value);
                        else if (name == "spawn_y") spawnY = std::stof(value);
                        else if (name == "spawn_w") spawnW = std::stof(value);
                        else if (name == "spawn_h") spawnH = std::stof(value);
                    }

                    auto entity = createButton(registry, o.x, o.y, channel);
                    auto& btn = registry.get<Button>(entity);

                    // LÓGICA DEL SPAWN DE PLATAFORMA
                    if (actionType == "spawn_platform") {
                        // AHORA SÍ FUNCIONA: scaleX y scaleY están declaradas arriba
                        spawnX *= scaleX;
                        spawnY *= scaleY;
                        spawnW *= scaleX;
                        spawnH *= scaleY;

                        btn.onPressAction = [spawnX, spawnY, spawnW, spawnH](entt::registry& reg) {
                            std::cout << "¡Puzzle resuelto! Creando plataforma." << std::endl;
                            createPlatform(reg, spawnX, spawnY, spawnW, spawnH, 0.0f, 0.0f, GOLD); 
                        };
                    }
                    // LÓGICA DE ABRIR PUERTA
                    else if (actionType == "abrir_puerta_final") {
                        btn.onPressAction = [](entt::registry& reg) {
                             // AHORA SÍ FUNCIONA: Se ha incluido "../components/Door.hpp"
                             auto view = reg.view<Door>();
                             for(auto e : view) {
                                 // Quitar la colisión sólida para que se pueda pasar
                                 if (reg.any_of<Solid>(e)) {
                                     reg.remove<Solid>(e);
                                 }
                                 std::cout << "Puerta abierta" << std::endl;
                             }
                        };
                    }
                }
            }
            // --- OBJETOS ESTÁTICOS SIN PROPIEDADES ---
            else 
            {
                if (o.type == "Platform") 
                    createPlatform(registry, o.x, o.y, o.width, o.height, 0.0f, 0.0f, DARKGRAY);
                else if (o.type == "Door") 
                    createDoor(registry, o.x, o.y, o.width, o.height, GREEN);
                else if(o.type == "Spike")
                    createSpike(registry, o.x, o.y, o.width, o.height, spikeTex);
                else if(o.type == "Wheel")
                    createWheel(registry, o.x, o.y, o.width / 2.0f, wheelTex);
            }
        }
    }
}


// --------------------------------------------------------------------------------------------------------------------------------------------
// Definición -> estas funciones lo que hacen es introducir una funcion anonima (lambdas) en el vector de acciones y condiciones de las trampas
// --------------------------------------------------------------------------------------------------------------------------------------------

//======================FUNCIONES RELACIONADAS CON LAS TRAMPAS============================

static void ProcessTrap(entt::registry& registry, entt::entity entity,
                       const std::string& conditionType, float conditionValue,
                       const std::string& actionType, float actionValue,
                       float speed)
{
    // Setear velocidad si existe
    if (registry.any_of<Velocity>(entity) && speed != 0.f)
    {
        auto &vel = registry.get<Velocity>(entity);
        vel.vx = speed;
        vel.vy = speed;
    }

    // Añadir condición
    if (conditionType == "proximity")
        AddProximityCondition(registry, entity, conditionValue);
    else if (conditionType == "timer")
        AddTimerCondition(registry, entity, conditionValue);

    // Añadir acción
    if (actionType == "move_horizontal")
        AddMoveHorizontalAction(registry, entity, actionValue);
    else if (actionType == "move_vertical")
        AddMoveVerticalAction(registry, entity, actionValue);
    else if (actionType == "velocity_horizontal")
        AddVelocityHorizontalAction(registry, entity, actionValue);
    else if (actionType == "velocity_vertical")
        AddVelocityVerticalAction(registry, entity, actionValue);
    else if (actionType == "dimension")
        AddChangeDimensionAction(registry, entity, actionValue);
}

//---------------------------------CONDICIONES--------------------------------------------
static void AddProximityCondition(entt::registry& registry, entt::entity e, float distance)
{
    auto &trap = registry.get<Trap>(e);

    trap.conditions.push_back(
        [e, distance, &registry](float) -> bool
        {
            if (!registry.any_of<Position>(e) || !registry.any_of<Solid>(e))
                return false;

            auto players = registry.view<Player, Position>();
            if (players.begin() == players.end()) return false; // no hay players

            auto &trapPos = registry.get<Position>(e);
            auto &solid   = registry.get<Solid>(e);

            float minDist = FLT_MAX;

            for (auto p : players)
            {
                auto &ppos = players.get<Position>(p);

                float closestX = fmaxf(trapPos.x, fminf(ppos.x, trapPos.x + solid.width));
                float closestY = fmaxf(trapPos.y, fminf(ppos.y, trapPos.y + solid.height));

                float dx = ppos.x - closestX;
                float dy = ppos.y - closestY;
                float dist = sqrtf(dx*dx + dy*dy);

                if (dist < minDist) minDist = dist;
            }

            return (minDist <= distance);
        }
    );
}

static void AddTimerCondition(entt::registry& registry, entt::entity e, float delay)
{
    auto &trap = registry.get<Trap>(e);

    trap.conditions.push_back(
        [delay, elapsed = 0.0f](float dt) mutable -> bool
        {
            elapsed += dt;
            return (elapsed >= delay);
        }
    );
}

//---------------------------------ACCIONES--------------------------------------------
static void AddMoveHorizontalAction(entt::registry& registry, entt::entity e, float amount)
{
    auto &trap = registry.get<Trap>(e);

    trap.actions.push_back(
        [e, amount, &registry](float dt) mutable -> bool
        {
            auto& trapVel = registry.get<Velocity>(e);
            auto& trapPos = registry.get<Position>(e);

            if(amount < 0 && trapVel.vx > 0) trapVel.vx *= -1;

            trapPos.x += trapVel.vx;

            amount -= trapVel.vx;

            return (amount == 0.f);
        }
    );
}

static void AddMoveVerticalAction(entt::registry& registry, entt::entity e, float amount)
{
    auto &trap = registry.get<Trap>(e);

    trap.actions.push_back(
        [e, amount, &registry](float dt) mutable -> bool
        {
            auto& trapVel = registry.get<Velocity>(e);
            auto& trapPos = registry.get<Position>(e);

            if(amount < 0 && trapVel.vy > 0) trapVel.vy *= -1;

            trapPos.y += trapVel.vy;

            amount -= trapVel.vy;

            return (amount == 0.f);
        }
    );
}

static void AddVelocityHorizontalAction(entt::registry& registry, entt::entity e, float newVx)
{
    auto &trap = registry.get<Trap>(e);

    trap.actions.push_back(
        [e, newVx, &registry](float) -> bool
        {
            if (!registry.any_of<Velocity>(e)) return true;
            auto &vel = registry.get<Velocity>(e);
            vel.vx = newVx;
            return true; // instantáneo
        }
    );
}

static void AddVelocityVerticalAction(entt::registry& registry, entt::entity e, float newVy)
{
    auto &trap = registry.get<Trap>(e);

    trap.actions.push_back(
        [e, newVy, &registry](float) -> bool
        {
            if (!registry.any_of<Velocity>(e)) return true;
            auto &vel = registry.get<Velocity>(e);
            vel.vy = newVy;
            return true;
        }
    );
}

static void AddChangeDimensionAction(entt::registry& registry, entt::entity e, float amount)
{
    auto &trap = registry.get<Trap>(e);

    trap.actions.push_back(
        [e, amount, &registry](float ) mutable -> bool
        {
            if (!registry.any_of<Solid>(e) || !registry.any_of<Position>(e))
                return true;

            auto &size = registry.get<Solid>(e);
            auto &pos  = registry.get<Position>(e);

            float step = 10.0f;
            if (amount < 0) step *= -1.0f;

            size.width -= step;
            pos.x += step * 0.5f;

            amount -= step;

            if(amount == 0.0f) return true;

            return false;
        }
    );
}

/*static void AddSpawnAction(entt::registry& registry, entt::entity e, const std::string& enemyType)
{
    auto &trap = registry.get<Trap>(e);

    trap.actions.push_back(
        [enemyType, e, &registry](float) -> bool
        {
            
            return true; 
        }
    );
}
*/


//====================FUNCIONES RELACIONADAS CON LAS LÓGICA DE BUCLES============================

// -----------------------------------------
// Movimiento en bucle horizontal
// -----------------------------------------
static void AddPatronLoopHorizontal(entt::registry& registry, entt::entity e, float amount)
{
    auto &p = registry.get_or_emplace<Patron>(e);

    p.actions.push_back(
        [e, amount, traveled = 0.0f, forward = true, &registry](float dt) mutable
        {
            if (!registry.any_of<Velocity>(e) || !registry.any_of<Position>(e))
                return;

            auto &vel = registry.get<Velocity>(e);
            auto &pos = registry.get<Position>(e);

            if (forward)
            {
                pos.x += vel.vx * dt;
                traveled += std::abs(vel.vx * dt);

                if (traveled >= amount)
                {
                    vel.vx *= -1;
                    forward = false;
                    traveled = 0;
                }
            }
            else
            {
                pos.x += vel.vx * dt;
                traveled += std::abs(vel.vx * dt);

                if (traveled >= amount)
                {
                    vel.vx *= -1;
                    forward = true;
                    traveled = 0;
                }
            }
        }
    );
}

// -----------------------------------------
// Movimiento en bucle vertical
// -----------------------------------------
static void AddPatronLoopVertical(entt::registry& registry, entt::entity e, float amount)
{
    auto &p = registry.get_or_emplace<Patron>(e);

    p.actions.push_back(
        [e, amount, traveled = 0.0f, forward = true, &registry](float dt) mutable
        {
            if (!registry.any_of<Velocity>(e) || !registry.any_of<Position>(e))
                return;

            auto &vel = registry.get<Velocity>(e);
            auto &pos = registry.get<Position>(e);

            if (forward)
            {
                pos.y += vel.vy * dt;
                traveled += std::abs(vel.vy * dt);

                if (traveled >= amount)
                {
                    vel.vy *= -1;
                    forward = false;
                    traveled = 0;
                }
            }
            else
            {
                pos.y += vel.vy * dt;
                traveled += std::abs(vel.vy * dt);

                if (traveled >= amount)
                {
                    vel.vy *= -1;
                    forward = true;
                    traveled = 0;
                }
            }
        }
    );
}

//====================FUNCIONES RELACIONADAS CON LOS BOTONES============================