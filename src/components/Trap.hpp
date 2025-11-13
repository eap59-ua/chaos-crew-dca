#pragma once

enum class Condition {
    None,
    Proximity,
    Timer,
    PressurePlate,
    Manual,
};

struct Trap {
    Condition activationCondition = Condition::None;

    float activationRadius = 0.0f; // For Proximity condition
};