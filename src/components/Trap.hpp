#pragma once
#include <functional>
#include <vector>

struct Trap {
    bool triggered = false;   // la condición se ha cumplido (se activó la trampa)
    bool finished  = false;   // todas las acciones han terminado

    std::vector<std::function<bool(float)>> conditions;
    std::vector<std::function<bool(float)>> actions;
};