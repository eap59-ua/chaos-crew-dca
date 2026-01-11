#define BOOST_TEST_MODULE MapTests
#include <boost/test/included/unit_test.hpp>
#include <string>
#include <vector>
#include <sstream>

// ============================================================================
// SISTEMA DE MAPAS (CASOS 2 y 3: Fuentes del juego + Carga de mapas)
// ============================================================================
// Este código simula tu sistema de carga de mapas del juego

struct TileMap {
    int width, height;
    int tileWidth, tileHeight;
    std::vector<int> tiles;
    bool isLoaded;
    
    TileMap() : width(0), height(0), tileWidth(0), tileHeight(0), isLoaded(false) {}
    
    // Cargar mapa desde formato CSV (simplificado)
    bool loadFromData(const std::string& data) {
        std::istringstream stream(data);
        std::string line;
        
        // PASO 1: Leer dimensiones (formato: width,height,tileWidth,tileHeight)
        if (!std::getline(stream, line)) return false;
        
        std::istringstream dimStream(line);
        char comma;
        if (!(dimStream >> width >> comma >> height >> comma >> tileWidth >> comma >> tileHeight)) {
            return false;
        }
        
        // PASO 2: Validar dimensiones (importante para evitar crashes)
        if (width <= 0 || height <= 0 || tileWidth <= 0 || tileHeight <= 0) {
            return false;
        }
        
        // PASO 3: Leer tiles
        tiles.clear();
        tiles.reserve(width * height);
        
        while (std::getline(stream, line)) {
            std::istringstream lineStream(line);
            int tile;
            while (lineStream >> tile) {
                tiles.push_back(tile);
                if (lineStream.peek() == ',') lineStream.ignore();
            }
        }
        
        // PASO 4: Verificar que tenemos suficientes tiles
        if (tiles.size() != static_cast<size_t>(width * height)) {
            return false;
        }
        
        isLoaded = true;
        return true;
    }
    
    // Obtener tile en posición (x, y)
    int getTile(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return -1;  // Indicador de fuera de límites
        }
        return tiles[y * width + x];
    }
};

// ============================================================================
// TEST #4: Cargar mapa válido (CASOS 2 y 3)
// ============================================================================
// ¿Qué testea? Que se puede cargar un mapa con formato correcto
// ¿Por qué? Los niveles del juego se cargan desde archivos
// Teoría: Unit test, BlackBox (no miramos cómo se parsea internamente)

BOOST_AUTO_TEST_CASE(test_load_valid_map) {
    // ===== ARRANGE =====
    TileMap map;
    
    // Formato de mapa: 3x3 tiles de 32x32 píxeles
    std::string validMapData = 
        "3,3,32,32\n"      // Dimensiones
        "1,2,3\n"          // Fila 1
        "4,5,6\n"          // Fila 2
        "7,8,9";           // Fila 3
    
    // ===== ACT =====
    bool result = map.loadFromData(validMapData);
    
    // ===== ASSERT =====
    // Verificaciones múltiples (principio: Self-validating)
    BOOST_TEST(result, "Valid map should load successfully");
    BOOST_TEST(map.isLoaded, "Map should be marked as loaded");
    BOOST_TEST(map.width == 3);
    BOOST_TEST(map.height == 3);
    BOOST_TEST(map.tileWidth == 32);
    BOOST_TEST(map.tileHeight == 32);
    BOOST_TEST(map.tiles.size() == 9);  // 3x3 = 9 tiles
}

// ============================================================================
// TEST #5: Verificar integridad de datos del mapa (CASOS 2 y 3)
// ============================================================================
// ¿Qué testea? Que los datos del mapa se almacenan correctamente
// ¿Por qué? Si los tiles están en posiciones incorrectas, el nivel se ve mal
// Teoría: Unit test, WhiteBox (conocemos la estructura interna)

BOOST_AUTO_TEST_CASE(test_map_data_integrity) {
    // ===== ARRANGE =====
    TileMap map;
    
    // Mapa 2x2 con valores específicos
    std::string mapData = 
        "2,2,16,16\n"
        "10,20\n"
        "30,40";
    
    // ===== ACT =====
    map.loadFromData(mapData);
    
    // ===== ASSERT =====
    // Verificar cada tile individualmente
    // Estructura del mapa:
    //   x=0  x=1
    // y=0: 10   20
    // y=1: 30   40
    
    BOOST_TEST(map.getTile(0, 0) == 10, "Top-left tile should be 10");
    BOOST_TEST(map.getTile(1, 0) == 20, "Top-right tile should be 20");
    BOOST_TEST(map.getTile(0, 1) == 30, "Bottom-left tile should be 30");
    BOOST_TEST(map.getTile(1, 1) == 40, "Bottom-right tile should be 40");
    
    // Verificar acceso fuera de límites (edge case)
    BOOST_TEST(map.getTile(-1, 0) == -1, "Out of bounds should return -1");
    BOOST_TEST(map.getTile(0, -1) == -1, "Out of bounds should return -1");
    BOOST_TEST(map.getTile(2, 0) == -1, "Out of bounds should return -1");
    BOOST_TEST(map.getTile(0, 2) == -1, "Out of bounds should return -1");
}

// ============================================================================
// NOTAS TEÓRICAS - VALIDACIÓN DE RECURSOS
// ============================================================================
/*
¿Por qué es importante testear la carga de recursos?
1. Los archivos pueden estar corruptos
2. El formato puede cambiar
3. Los bugs en parsing son difíciles de detectar en runtime
4. Previene crashes por datos inválidos

Casos típicos a testear:
✅ Carga exitosa con datos válidos (Test #4)
✅ Integridad de datos (Test #5)
✅ Rechazo de datos inválidos (dimensiones negativas, datos incompletos)
✅ Manejo de edge cases (acceso fuera de límites)

Tipos de testing aplicados:
- BlackBox: Verificamos que la función cumple su especificación
- WhiteBox: Verificamos la estructura interna de datos

Ejemplo de error común SIN test:
void GameLevel::render() {
    for (int y = 0; y <= height; y++) {  // BUG: <= debería ser <
        for (int x = 0; x <= width; x++) {
            drawTile(map.getTile(x, y));  // CRASH: acceso fuera de límites
        }
    }
}

Con Test #5, este bug se detecta ANTES de ejecutar el juego.
*/