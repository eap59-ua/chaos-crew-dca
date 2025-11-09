#include "WinSystem.hpp"
#include "../components/Door.hpp"
#include "../components/Sprite.hpp"
#include "../components/Player.hpp"
#include "../components/Position.hpp"
#include "../components/Velocity.hpp"
#include "../components/Solid.hpp"
#include <iostream>
#include <raylib.h>
using namespace std;
bool CheckVictory(entt::registry& registry){
    auto doors = registry.view<Door, Position, Solid, Sprite>();
    auto players = registry.view<Player, Position, Velocity, Solid, Sprite>();
    
	for (auto playerEntity : players) {
    	auto &ppos = players.get<Sprite>(playerEntity);
    	for (auto doorEntity : doors) {
        	auto &dpos = doors.get<Sprite>(doorEntity);
        	if(!CheckCollisionRecs(ppos.rect, dpos.rect)) return false;
    	}
	}

    return true;
}


bool CheckDefeat(entt::registry& registry){
    auto players = registry.view<Player, Position, Velocity, Solid, Sprite>();
    
	for (auto playerEntity : players) {
    	auto &ppos = players.get<Position>(playerEntity);
    	auto &solid = players.get<Solid>(playerEntity);
    	if(ppos.y >= 720 - solid.height) return true;
	}

    return false;
}
