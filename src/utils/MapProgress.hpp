#pragma once
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

namespace MapProgress {
    // Lee / guarda progreso en "mapas/progress.txt" (una ruta por línea)
    unordered_set<string> Load();
    void Save(const unordered_set<string>& done);

    // Devuelve lista ordenada de mapas (*.xml) en /mapas (rutas relativas)
    vector<string> GetAvailableMaps();

    // Marca un mapa como completado y persiste
    void MarkCompleted(const string& mapPath);
}
