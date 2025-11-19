#include "TrollSystem.hpp"
#include "../components/Trap.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"

#include "../entt/entt.hpp"
#include <vector>

void TrapSystem(entt::registry& registry, float dt)
{
    // Vista de traps (solo Trap es obligatorio)
    auto view = registry.view<Trap>();

    for (auto entity : view)
    {
        auto &trap = view.get<Trap>(entity);

        // Si ya terminó -> eliminar componente Trap
        if (trap.finished) {
            if (registry.any_of<Trap>(entity))
                registry.remove<Trap>(entity);
            continue;
        }

        // 1) Si no está activada, evaluar condiciones
        if (!trap.triggered)
        {
            for (auto &cond : trap.conditions)
            {
                // Si alguna condición devuelve true -> activamos la trampa
                if (cond(dt)) {
                    trap.triggered = true;
                    break;
                }
            }

            if (!trap.triggered)
                continue; // aún no activada, siguiente trampa
        }

        // 2) Ejecutar acciones
        bool allDone = true;
        for (auto &action : trap.actions)
        {
            bool done = action(dt);
            if (!done) allDone = false;
        }

        trap.finished = allDone;
    }
}
