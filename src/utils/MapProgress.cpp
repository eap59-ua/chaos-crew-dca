#include "MapProgress.hpp"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <regex>

using namespace std;

static const char* kProgressFile = "mapas/progress.txt";
static const char* kMapsDir      = "mapas";

namespace MapProgress {

unordered_set<string> Load() { 
    unordered_set<string> done;
    ifstream in(kProgressFile); 
    if (!in) return done;
    string line;
    while (getline(in, line)) { 
        if (!line.empty()) done.insert(line);
    }
    return done;
}

void Save(const unordered_set<string>& done) {
    ofstream out(kProgressFile, ios::trunc);
    if (!out) return;
    for (const auto& m : done) out << m << "\n";
}

// Función auxiliar para extraer el número del nombre del mapa
static int ExtractMapNumber(const string& mapPath) {
    // Buscar patrón "mapa" seguido de dígitos
    regex pattern(R"(mapa(\d+)\.xml)");
    smatch match;
    
    if (regex_search(mapPath, match, pattern) && match.size() > 1) {
        return stoi(match[1].str());
    }
    
    return -1; // Si no encuentra número, devolver -1
}

vector<string> GetAvailableMaps() {
    namespace fs = filesystem;
    vector<string> maps;
    if (!fs::exists(kMapsDir)) return maps;

    for (auto& entry : fs::directory_iterator(kMapsDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".xml") {
            maps.push_back(entry.path().generic_string());
        }
    }
    
    // ORDENAMIENTO NATURAL (numérico)
    sort(maps.begin(), maps.end(), [](const string& a, const string& b) {
        int numA = ExtractMapNumber(a);
        int numB = ExtractMapNumber(b);
        
        // Si ambos tienen números válidos, comparar numéricamente
        if (numA >= 0 && numB >= 0) {
            return numA < numB;
        }
        
        // Fallback: orden alfabético si no se puede extraer número
        return a < b;
    });
    
    return maps;
}

void MarkCompleted(const string& mapPath) {
    auto done = Load();
    done.insert(mapPath);
    Save(done);
}

} // namespace MapProgress