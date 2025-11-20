#include "TrollSystem.hpp"
#include "../components/Trap.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"

#include "../entt/entt.hpp"
#include <vector>
#include <algorithm> // for std::min, std::all_of

void TrapSystem(entt::registry& registry, float dt)
{
    auto view = registry.view<Trap>();

    for (auto entity : view)
    {
        auto &trap = view.get<Trap>(entity);

        // Inicializar buffers si están vacíos
        if (trap.triggeredPerCondition.size() != trap.conditions.size())
            trap.triggeredPerCondition.assign(trap.conditions.size(), false);

        if (trap.finishedPerAction.size() != trap.actions.size())
            trap.finishedPerAction.assign(trap.actions.size(), false);

        // Evaluar condiciones por índice
        for (size_t i = 0; i < trap.conditions.size(); ++i)
        {
            // Si ya se activó esa condición, saltar
            if (trap.triggeredPerCondition[i])
                continue;

            // Si ahora se cumple → marcar
            if (trap.conditions[i](dt))
                trap.triggeredPerCondition[i] = true;
        }

        // Ejecutar acciones correspondientes
        for (size_t i = 0; i < trap.actions.size(); ++i)
        {
            // Si la condición[i] nunca se cumplió -> saltar
            if (!trap.triggeredPerCondition[i])
                continue;

            // Si la acción[i] ya terminó -> saltar
            if (trap.finishedPerAction[i])
                continue;

            // Ejecutar acción
            bool done = trap.actions[i](dt);
            if (done)
                trap.finishedPerAction[i] = true;
        }

        // terminaron todas?
        bool allDone = true;
        for (bool fin : trap.finishedPerAction)
        {
            if (!fin) {
                allDone = false;
                break;
            }
        }

        // borrar el componente tramp
        if (allDone)
            registry.remove<Trap>(entity);
    }
}

