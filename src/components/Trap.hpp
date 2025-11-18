#pragma once

// ========Condiciones==========
struct ProximityCondition {
    float distance;
};

struct TimerCondition {
    float delay;
    float elapsed = 0.f;
};

struct PressurePlateCondition {
    bool pressed = false;
};

// =========Acciones=============
struct MoveAction {
    float dx, dy;
};

struct ChangeDimensionAction {
	float dw, dh;
};

struct ChangeVelocityAction {
    float newVelocityX, newVelocityY;
};

struct SpawnAction {
    std::string enemyType;
};

// ==========Trampa==============
struct Trap {
    bool conditionMet = false;
    bool actionMet = false;
    bool finMet = false;
};

