# 🤝 Guía de Contribución - Chaos Crew

¡Gracias por contribuir a Chaos Crew! Esta guía te ayudará a seguir nuestras convenciones y flujo de trabajo.

## 📋 Tabla de Contenidos

1. [Flujo de Trabajo (GitHub Flow)](#flujo-de-trabajo)
2. [Convenciones de Branches](#convenciones-de-branches)
3. [Convenciones de Commits](#convenciones-de-commits)
4. [Pull Requests](#pull-requests)
5. [Code Reviews](#code-reviews)
6. [Estilo de Código](#estilo-de-código)
7. [Testing](#testing)

---

## 🔄 Flujo de Trabajo

Usamos **GitHub Flow**, un flujo simple y efectivo:

```
1. main (siempre estable)
   ↓
2. Crear rama feature/bugfix
   ↓
3. Hacer commits
   ↓
4. Push y abrir Pull Request
   ↓
5. Code Review
   ↓
6. Merge a main
   ↓
7. Rama se elimina automáticamente
```

### Reglas de Oro

✅ **SIEMPRE:**

- Trabajar en ramas `feature/*` o `bugfix/*`
- Hacer commits frecuentes (varios al día)
- Abrir Pull Request cuando esté listo
- Esperar aprobación antes de merge
- Actualizar tu rama con `main` diariamente

❌ **NUNCA:**

- Commits directos a `main` (está protegida)
- Trabajar directamente en `main`
- Hacer merge sin aprobación
- Ignorar comentarios de code review

---

## 🌿 Convenciones de Branches

### Formato

```
tipo/descripcion-kebab-case
```

### Tipos de Ramas

| Prefijo     | Uso                    | Ejemplo                      |
| ----------- | ---------------------- | ---------------------------- |
| `feature/`  | Nueva funcionalidad    | `feature/player-movement`    |
| `bugfix/`   | Corrección de errores  | `bugfix/collision-detection` |
| `docs/`     | Solo documentación     | `docs/hito1-gdd`             |
| `refactor/` | Refactorización        | `refactor/physics-module`    |
| `test/`     | Añadir/modificar tests | `test/collision-unit-tests`  |

### Ejemplos Correctos

```bash
✅ feature/player-jump-mechanic
✅ bugfix/player-fall-through
✅ docs/update-readme
✅ refactor/separate-entities
✅ test/add-physics-tests
```

### Ejemplos Incorrectos

```bash
❌ fix-bug                    # Demasiado genérico
❌ erardo-changes             # Incluye nombre personal
❌ my_new_feature             # Snake_case en vez de kebab-case
❌ FEATURE/PLAYER             # Mayúsculas
```

### Ciclo de Vida de una Rama

```bash
# 1. Actualizar main
git checkout main
git pull origin main

# 2. Crear rama
git checkout -b feature/player-movement

# 3. Trabajar...
git add .
git commit -m "feat: implement basic player movement"

# 4. Push
git push origin feature/player-movement

# 5. Abrir PR en GitHub

# 6. Tras merge, actualizar main local
git checkout main
git pull origin main
```

---

## 💬 Convenciones de Commits

Seguimos **Conventional Commits** para mensajes claros y consistentes.

### Formato

```
tipo: descripción breve en minúsculas

Ejemplos:
feat: add player jump mechanic
fix: resolve collision detection bug
docs: update README with build instructions
```

### Tipos de Commits

| Tipo        | Uso                 | Ejemplo                            |
| ----------- | ------------------- | ---------------------------------- |
| `feat:`     | Nueva funcionalidad | `feat: implement player jump`      |
| `fix:`      | Corrección de bug   | `fix: correct collision detection` |
| `docs:`     | Documentación       | `docs: update README`              |
| `style:`    | Formateo, espacios  | `style: format code`               |
| `refactor:` | Refactorización     | `refactor: extract physics module` |
| `test:`     | Tests               | `test: add collision tests`        |
| `chore:`    | Build, dependencias | `chore: update raylib`             |

### Ejemplos Correctos

```bash
✅ feat: add jump mechanic with gravity
✅ fix: correct player-platform collision
✅ docs: add GDD section to HITO1.md
✅ refactor: split main.cpp into modules
✅ test: add unit tests for physics
```

### Ejemplos Incorrectos

```bash
❌ fixed bug                           # No descriptivo
❌ update                              # Demasiado vago
❌ Add jump and fix collision and docs # Múltiples cambios
❌ FEAT: New Feature                   # Mayúsculas incorrectas
```

### Mensajes de Commit Detallados

Para commits complejos, usa descripción extendida:

```bash
git commit -m "feat: implement cooperative exit detection

- Add exitZone collision check for both players
- Require both players to reach exit for victory
- Add visual feedback when players reach exit
- Update game state transitions

Closes #5"
```

---

## 🔀 Pull Requests

### Crear un Pull Request

1. **Push tu rama:**

   ```bash
   git push origin feature/mi-feature
   ```

2. **En GitHub:**
   - Click "Compare & pull request"
   - Usar el template automático
   - Título descriptivo
   - Completar toda la información

### Template de PR (Se Auto-Completa)

```markdown
## Descripción

[Descripción de los cambios]

## Tipo de cambio

- [ ] Nueva funcionalidad (feature)
- [ ] Corrección de bug (bugfix)
- [ ] Documentación (docs)
- [ ] Refactorización (refactor)

## Testing realizado

- [ ] Compila sin errores
- [ ] Probado en Linux
- [ ] Sin warnings
- [ ] Funcionalidad testeada

## Checklist

- [ ] El código sigue las convenciones
- [ ] He comentado código complejo
- [ ] He actualizado documentación
- [ ] No hay conflictos con main

## Cierra issue

Closes #[número]
```

### Ejemplo de PR Completo

```markdown
## Descripción

Implementa el sistema de movimiento básico para ambos jugadores con controles
de teclado independientes (WASD para P1, flechas para P2).

## Tipo de cambio

- [x] Nueva funcionalidad (feature)

## Testing realizado

- [x] Compila sin errores
- [x] Probado en Linux
- [x] Sin warnings
- [x] Funcionalidad testeada manualmente 50+ veces

## Cambios específicos

- Estructura Player con posición, velocidad, color
- Input handling para 2 jugadores simultáneos
- Movimiento horizontal suave
- Restricción en límites de pantalla

## Screenshots

[Imagen del juego funcionando]

## Checklist

- [x] El código sigue las convenciones
- [x] He comentado partes complejas
- [x] No hay conflictos con main

Closes #2
```

---

## 👀 Code Reviews

### Para el Autor del PR

**Antes de solicitar review:**

- [ ] El código compila sin errores ni warnings
- [ ] Has probado la funcionalidad exhaustivamente
- [ ] Has actualizado documentación si es necesario
- [ ] El PR tiene descripción clara y completa

**Durante el review:**

- Responde a todos los comentarios
- Sé receptivo al feedback constructivo
- Haz los cambios solicitados prontamente
- Marca conversaciones como resueltas cuando aplique

### Para el Reviewer

**Al revisar código:**

1. **Lee el código cuidadosamente:**

   - ¿Es claro y legible?
   - ¿Sigue las convenciones?
   - ¿Hay bugs evidentes?

2. **Prueba el código localmente:**

   ```bash
   git fetch origin
   git checkout feature/nombre-rama
   ./build.sh linux
   ./bin/chaos-crew
   ```

3. **Deja comentarios constructivos:**
   ✅ "Considera usar un const aquí para mayor claridad"
   ✅ "Buen trabajo con el manejo de colisiones!"
   ❌ "Esto está mal"
   ❌ "No me gusta"

4. **Decide:**
   - ✅ **Approve** - Todo está bien
   - 💬 **Comment** - Sugerencias menores
   - ❌ **Request changes** - Cambios necesarios

### Tiempo de Respuesta

- **Revisión inicial:** <24 horas
- **Re-revisión tras cambios:** <12 horas
- **Merge tras aprobación:** Inmediato

---

## 💅 Estilo de Código

### C++ General

```cpp
// ✅ Correcto
constexpr float GRAVITY = 0.6f;
const int MAX_PLAYERS = 5;

struct Player {
    Vector2 position;
    Vector2 velocity;
    Color color;
    bool isGrounded;
};

void updatePlayer(Player& player) {
    if (player.isGrounded) {
        player.velocity.y = 0;
    }
}

// ❌ Incorrecto
float gravity = 0.6f;  // No constexpr
int maxplayers = 5;    // Naming inconsistente

struct player {        // Minúsculas para struct
    Vector2 pos;      // Nombres abreviados
    Vector2 vel;
};
```

### Naming Conventions

| Elemento            | Convención              | Ejemplo                         |
| ------------------- | ----------------------- | ------------------------------- |
| **Constants**       | UPPER_SNAKE_CASE        | `SCREEN_WIDTH`                  |
| **Variables**       | camelCase               | `playerVelocity`                |
| **Functions**       | camelCase               | `updatePlayer()`                |
| **Structs/Classes** | PascalCase              | `Player`, `Platform`            |
| **Files**           | PascalCase o kebab-case | `Player.cpp` o `game-state.cpp` |

### Comentarios

```cpp
// ✅ Comentarios útiles
// Apply gravity when player is not grounded
if (!player.isGrounded) {
    player.velocity.y += GRAVITY;
}

// Complex collision resolution using AABB method
// We check all 4 corners to handle edge cases
resolveCollision(player, platform);

// ❌ Comentarios obvios
// Set x to 0
x = 0;

// Loop
for (int i = 0; i < 10; i++) {
    // ...
}
```

### Indentación y Formato

- **Espacios:** 4 espacios (no tabs)
- **Llaves:** Estilo K&R

```cpp
// ✅ Correcto
void function() {
    if (condition) {
        doSomething();
    } else {
        doSomethingElse();
    }
}

// ❌ Incorrecto
void function()
{
    if(condition)
    {
        doSomething();
    }
    else
    {
        doSomethingElse();
    }
}
```

---

## 🧪 Testing

### Testing Manual (Hito 1)

Antes de abrir PR, probar:

```bash
# Compilar
./build.sh linux

# Testing básico
./bin/chaos-crew

# Checklist manual:
- [ ] El juego inicia sin errores
- [ ] Ambos jugadores se controlan correctamente
- [ ] Las colisiones funcionan
- [ ] La victoria/derrota se detecta
- [ ] Se puede reiniciar
- [ ] No hay crashes evidentes
- [ ] FPS estables (~60)
```

### Testing Automatizado (Hito 2+)

```cpp
// Ejemplo de test unitario (futuro)
TEST(PhysicsTest, GravityAppliedWhenNotGrounded) {
    Player player;
    player.velocity.y = 0;
    player.isGrounded = false;

    applyGravity(player);

    ASSERT_GT(player.velocity.y, 0);
}
```

---

## 📞 Comunicación

### Daily Standups

Cada día en el chat del equipo (~15 min):

```
Buenos días equipo:

Ayer: Implementé el sistema de salto
Hoy: Trabajaré en las colisiones
Bloqueantes: Ninguno
```

### Pedir Ayuda

```bash
# En el issue o PR
@compañero ¿Podrías revisar esta parte?
Tengo dudas sobre cómo manejar [X]

# O marcar el issue como "help-wanted"
```

---

## 🎯 Checklist Final

Antes de hacer tu primer commit, verifica:

- [ ] He leído esta guía completa
- [ ] Entiendo GitHub Flow
- [ ] Sé cómo nombrar branches y commits
- [ ] He configurado mi entorno (vendor/, raylib)
- [ ] Sé cómo compilar el proyecto
- [ ] Conozco el proceso de PR y code review

---

## 🆘 ¿Dudas?

- **Git/GitHub:** Preguntar a Erardo (@eap59-ua)
- **Código:** Preguntar en el chat del equipo
- **Proceso:** Revisar esta guía o preguntar en daily standup

---

**¡Bienvenido al equipo!** 🎮

Si tienes sugerencias para mejorar esta guía, abre un issue con el label `documentation`.

---

**Última actualización:** 9 de octubre de 2025  
**Equipo:** Chaos Crew - DCA Universidad de Alicante
