#pragma once
#include <functional>
#include <vector>

struct Trap {
    std::vector<std::function<bool(float)>> conditions;
    std::vector<std::function<bool(float)>> actions;

    std::vector<bool> triggeredPerCondition; // true si la condición i se ha cumplido
    std::vector<bool> finishedPerAction;     // true si la acción i ha terminado

    // helper opcional para comprobar rápido si el trap ya está en estado final
    bool isEmpty() const {
        return conditions.empty() && actions.empty();
    }
};
