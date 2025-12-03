#include "MapProgress.hpp"
#include <filesystem>
#include <fstream>
#include <algorithm>

using namespace std;

static const char* kProgressFile = "mapas/progress.txt"; // archivo de progreso, en este archivo se guardan las rutas relativas de los mapas completados
static const char* kMapsDir      = "mapas";

namespace MapProgress {

unordered_set<string> Load() { 
    unordered_set<string> done;
    ifstream in(kProgressFile); 
    if (!in) return done; // si no existe, devolvemos conjunto vacío
    string line;
    while (getline(in, line)) { 
        if (!line.empty()) done.insert(line); //insertar ruta del mapa completado
    }
    return done;
}

void Save(const unordered_set<string>& done) {
    ofstream out(kProgressFile, ios::trunc);
    if (!out) return;
    for (const auto& m : done) out << m << "\n"; //guardamos
}

vector<string> GetAvailableMaps() {
    namespace fs = filesystem;
    vector<string> maps;
    if (!fs::exists(kMapsDir)) return maps;

    for (auto& entry : fs::directory_iterator(kMapsDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".xml") {
            // Guardamos ruta relativa tipo "mapas/mapa1.xml"
            maps.push_back(entry.path().generic_string());
        }
    }
    sort(maps.begin(), maps.end()); // orden alfabético
    return maps;
}

void MarkCompleted(const string& mapPath) {
    auto done = Load();
    done.insert(mapPath);
    Save(done);
}

} // namespace MapProgress
