# Hito 1 - Alpha Build

## Chaos Crew - Plataformas Cooperativo 2D

**Fecha de entrega:** 29 de octubre de 2025  
**Versión del juego:** v0.1.0-alpha  
**Universidad de Alicante** - Desarrollo Colaborativo de Aplicaciones

---

## Equipo de Desarrollo

| Nombre               | Usuario GitHub       | Rol                   |
| -------------------- | -------------------- | --------------------- |
| Erardo Aldana Pessoa | @eap59-ua            | Project Lead / DevOps |
| Ángel Gonjar Verdejo | @agv114-ua           | Gameplay Programmer   |
| Rachid Mouradi       | @mouradirachid8-lgtm | Systems Programmer    |
| Youssef S.A.         | @ysa9-ua             | Level Designer        |
| Darío S.D.           | @dsd17-ua            | UI/Screens Developer  |

**Repositorio:** https://github.com/eap59-ua/chaos-crew-dca

---

# 1. GDD Básico (Game Design Document)

## 1.1 Concepto del Juego

**Chaos Crew** es un juego de plataformas 2D cooperativo donde **2 a 5 jugadores** deben trabajar en equipo para superar niveles llenos de obstáculos y trampas dinámicas.

| Atributo        | Valor                      |
| --------------- | -------------------------- |
| **Título**      | Chaos Crew                 |
| **Género**      | Plataformas 2D cooperativo |
| **Plataforma**  | PC (Linux/Windows)         |
| **Jugadores**   | 2-5 (multijugador local)   |
| **Tecnología**  | C++17 con Raylib 5.0       |
| **Perspectiva** | Vista lateral 2D           |

### Inspiración

- **Level Devil:** Mecánicas de trampas dinámicas e impredecibles
- **Pico Park:** Cooperación obligatoria entre jugadores

### Característica Distintiva

**Ambos jugadores deben llegar a la salida** para completar el nivel. Si uno falla, todos reinician, fomentando el trabajo en equipo genuino.

## 1.2 Mecánicas Principales

### Implementadas (Hito 1)

#### Movimiento del Jugador

- **Horizontal:** Velocidad constante (4.5 units/frame)
- **Salto:** Impulso inicial (-12.0 units) con gravedad (0.6 units/frame²)
- **Restricciones:** No pueden salir de los límites de pantalla

#### Sistema de Físicas

- **Gravedad:** Aplicada constantemente cuando no están en suelo
- **Estado "isGrounded":** Previene double jump
- **Velocidad vertical acumulada** por gravedad y salto

#### Colisiones

- **Detección AABB** (Axis-Aligned Bounding Box)
- **Resolución vertical:** Reposicionamiento encima de plataformas
- **Colisión con salida:** Detecta victoria cooperativa

#### Condiciones de Victoria/Derrota

- **Victoria:** ✅ **Ambos** jugadores en zona de salida
- **Derrota:** ❌ **Cualquier** jugador cae fuera de pantalla

### Planificadas (Hito 2+)

- Plataformas que desaparecen temporalmente
- Pinchos emergentes con timers
- Botones de presión múltiple (requieren 2+ jugadores)
- Plataformas activadas por peso
- Sistema de checkpoints
- Soporte para 3-5 jugadores con gamepads

## 1.3 Componentes del Juego

### Entidades Principales

| Entidad               | Descripción                      | Estado H1       |
| --------------------- | -------------------------------- | --------------- |
| **Jugador**           | Cuadrado controlable con físicas | ✅ Implementado |
| **Plataformas**       | Superficies estáticas            | ✅ Implementado |
| **Salida**            | Zona objetivo cooperativa        | ✅ Implementado |
| **Trampas dinámicas** | Plataformas móviles, pinchos     | 📅 Hito 2       |
| **Checkpoints**       | Puntos de guardado               | 📅 Hito 2       |

### Pantallas del Juego

| Pantalla           | Descripción                   | Estado H1       |
| ------------------ | ----------------------------- | --------------- |
| **Gameplay**       | Pantalla principal del juego  | ✅ Implementado |
| **Game Over**      | Victoria/Derrota con reinicio | ✅ Implementado |
| **Menú Principal** | Selección de modo             | 📅 Hito 2       |

## 1.4 Controles (Hito 1)

| Acción          | Jugador 1     | Jugador 2 |
| --------------- | ------------- | --------- |
| Mover izquierda | `A`           | `←`       |
| Mover derecha   | `D`           | `→`       |
| Saltar          | `W` o `SPACE` | `↑`       |
| Reiniciar       | `ENTER`       | `ENTER`   |

## 1.5 Estilo Visual (Hito 1)

**Figuras geométricas básicas** para prototipado:

| Elemento    | Representación    | Color  |
| ----------- | ----------------- | ------ |
| Jugador 1   | Cuadrado 30x40    | Azul   |
| Jugador 2   | Cuadrado 30x40    | Rojo   |
| Plataformas | Rectángulos       | Gris   |
| Salida      | Rectángulo 80x100 | Verde  |
| Fondo       | Sólido            | Blanco |

**Nota:** En Hito 2+ se implementarán sprites pixel art inspirados en Celeste y Super Meat Boy.

## 1.6 Alcance del Proyecto

### Hito 1 - Alpha (29 Oct 2025) ✅

- 2 jugadores simultáneos
- Mecánicas básicas de plataformas
- 1 nivel funcional
- 2 pantallas (Gameplay + Game Over)
- Gráficos con figuras geométricas

### Hito 2 - Beta (Mediados Nov 2025)

- 3-5 jugadores con gamepads
- Sistema de trampas dinámicas
- 3-4 niveles
- Sprites y audio
- Tests automatizados + CI/CD

### Hito 3 - Release (Principios Dic 2025)

- 5-8 niveles completos
- Patrón ECS (EnTT)
- Internacionalización (ES/EN)
- Sistema de logging
- Polish completo

---

# 2. Estrategia de Gestión de Ramas

## 2.1 Estrategia Elegida: GitHub Flow

### Descripción

GitHub Flow es un flujo de trabajo ligero basado en:

- **Una rama principal** (`main`) siempre en estado desplegable
- **Ramas temporales** para cada funcionalidad
- **Pull Requests obligatorios** con revisión de código

### Estructura de Ramas

```
main ──●────────●────────●────────●──── (siempre estable)
        │        │        │        │
        └────────┘        └────────┘
      feature/player  feature/physics
            │                │
         [PR + Review]  [PR + Review]
```

**Tipos de ramas:**

- `feature/*` - Nuevas funcionalidades
- `bugfix/*` - Correcciones de errores
- `docs/*` - Documentación
- `refactor/*` - Refactorización

## 2.2 Justificación de la Elección

### ¿Por qué GitHub Flow para nuestro proyecto?

#### 1. **Tamaño del Equipo (5 personas)**

Con 5 desarrolladores, no necesitamos la complejidad de Git Flow (diseñado para equipos de 50+ personas). GitHub Flow proporciona suficiente estructura sin overhead innecesario.

#### 2. **Ciclo de Entregas Frecuentes**

Tenemos **3 hitos en 6 semanas** (entregas cada ~3 semanas). GitHub Flow está diseñado para ciclos cortos con integración continua, permitiendo mantener `main` siempre lista para generar releases.

#### 3. **Naturaleza del Proyecto (Videojuego)**

El desarrollo de videojuegos requiere:

- Testing constante del gameplay
- Ajustes frecuentes basados en playtesting
- Iteración rápida sobre mecánicas

GitHub Flow facilita la integración rápida sin ventanas de release bloqueantes.

#### 4. **Integración Nativa con GitHub**

Al usar GitHub para hosting, obtenemos integración perfecta con:

- GitHub Issues (cierre automático con PRs)
- GitHub Projects (automatización del workflow)
- GitHub Actions (CI/CD en Hito 2)
- Branch protection (configuración nativa)

#### 5. **Code Reviews Obligatorios**

La protección de `main` con PRs obligatorios asegura:

- Todo código pasa por revisión de otro desarrollador
- Detección temprana de errores
- Conocimiento compartido del código
- Aprendizaje entre miembros del equipo

#### 6. **Curva de Aprendizaje Baja**

GitHub Flow es conceptualmente simple:

- `main` = código bueno y funcional
- `feature` = código en desarrollo
- `PR` = revisión antes de integrar

Todos los miembros pueden comprenderlo y aplicarlo desde el primer día.

### Comparación con Alternativas Descartadas

**Git Flow** ❌ DESCARTADO

- ❌ Complejidad innecesaria (5+ tipos de ramas)
- ❌ Overhead de gestión (mantener `develop` y `main`)
- ❌ Diseñado para software empresarial con múltiples versiones en producción
- ❌ Curva de aprendizaje alta para equipo mixto

**Trunk Based Development** ❌ DESCARTADO

- ❌ Requiere madurez en CI/CD desde día 1
- ❌ Ramas de muy corta vida (<24h) poco realistas para tareas complejas
- ❌ Alto riesgo de romper `main` sin barrera de PRs
- ❌ Dificulta code reviews adecuados

## 2.3 Implementación en el Proyecto

### Protecciones de Rama `main`

Configuradas en GitHub Settings:

- ✅ **Pull Request obligatorio** antes de merge
- ✅ **1 aprobación mínima** de otro miembro
- ✅ **Conversaciones resueltas** antes de merge
- ✅ **Eliminación automática** de ramas tras merge
- ✅ **Squash and merge** para historia limpia

### Convenciones de Nomenclatura

**Ramas:**

```
feature/player-movement      ✅ Correcto
feature/collision-system     ✅ Correcto
bugfix/player-fall-through   ✅ Correcto
docs/hito1-documentation     ✅ Correcto

fix-bug                      ❌ Demasiado genérico
my_new_feature               ❌ Snake_case
```

**Commits (Conventional Commits):**

```
feat: implement player jump mechanic           ✅
fix: correct collision detection               ✅
docs: update README with build instructions    ✅
refactor: extract physics to separate module   ✅

fixed bug                                      ❌ No descriptivo
update                                         ❌ Vago
```

### Workflow Típico

```bash
# 1. Actualizar main
git checkout main
git pull origin main

# 2. Crear rama feature
git checkout -b feature/player-movement

# 3. Trabajar y hacer commits
git add .
git commit -m "feat: add player structure"
# ... más commits

# 4. Subir rama
git push origin feature/player-movement

# 5. Abrir PR en GitHub
#    - Título descriptivo
#    - Descripción completa
#    - Asignar revisor
#    - Incluir "Closes #N" para cerrar issue

# 6. Code Review
#    - Revisor examina código
#    - Sugiere cambios si necesario
#    - Aprueba cuando está listo

# 7. Merge a main
#    - Squash and merge
#    - Issue se cierra automáticamente
#    - Rama se elimina automáticamente
```

---

# 3. Sistema de Bugtracking

## 3.1 Sistema Elegido: GitHub Issues + GitHub Projects

### Descripción

Utilizamos **GitHub Issues** integrado con **GitHub Projects** como sistema completo de gestión de bugs, features y tareas.

**GitHub Issues:**

- Crear issues (bugs, features, tareas)
- Asignar responsables
- Etiquetar con labels
- Comentar y discutir
- Cerrar automáticamente con commits/PRs

**GitHub Projects:**

- Vista Kanban del estado de tareas
- Automatización del workflow
- Visualización del progreso

## 3.2 Justificación de la Elección

### ¿Por qué GitHub Issues para nuestro proyecto?

#### 1. **Centralización Completa**

TODO está en GitHub:

- ✅ Código fuente (repositorio)
- ✅ Control de versiones (Git)
- ✅ Code reviews (Pull Requests)
- ✅ Gestión de tareas (Issues)
- ✅ Planificación (Projects)
- ✅ CI/CD (Actions en Hito 2)

**Beneficio:** No hay dispersión de información, todo en una sola plataforma.

#### 2. **Trazabilidad Directa entre Código y Bugs**

Integración nativa:

```bash
# En un commit
git commit -m "fix: correct collision detection (#8)"
# → El issue #8 se actualiza automáticamente

# En un Pull Request
Closes #8
# → Al hacer merge, el issue #8 se cierra automáticamente
```

**Resultado:**

- Ver exactamente qué código resolvió qué bug
- Historial completo de cambios relacionados
- Auditoría completa del ciclo de vida

#### 3. **Sin Coste ni Configuración Externa**

| Herramienta       | Coste           | Setup    | Mantenimiento |
| ----------------- | --------------- | -------- | ------------- |
| **Jira**          | 💰 Pago         | 2-3h     | Continuo      |
| **Trello**        | Gratis limitado | 1-2h     | Manual        |
| **Azure DevOps**  | Complejo        | 2-3h     | Alto          |
| **GitHub Issues** | ✅ Gratis       | ✅ 0 min | ✅ Ninguno    |

Para un proyecto académico de 6 semanas, no tiene sentido invertir tiempo en setup de herramientas externas.

#### 4. **Accesibilidad para Evaluadores**

**Ventaja académica crucial:**

- ✅ El profesor puede ver todo sin credenciales adicionales
- ✅ Auditar el proceso de desarrollo completo
- ✅ Verificar distribución de trabajo
- ✅ Comprobar uso correcto de la herramienta

Con herramientas externas:

- ❌ Evaluador necesitaría registrarse
- ❌ Nosotros dar permisos manualmente
- ❌ Exportar reports para demostrar uso

#### 5. **Templates y Estandarización**

Tenemos configurados **Issue Templates** que aseguran información completa:

**Bug Report Template:**

```markdown
## Descripción del Bug

[Qué está fallando]

## Pasos para Reproducir

1. ...
2. ...

## Comportamiento Esperado vs Actual

[Qué debería pasar vs qué pasa]

## Entorno

- OS: [Linux/Windows]
- Versión: [v0.1.0-alpha]
```

**Beneficio:** Información consistente, no olvidar detalles importantes.

#### 6. **Automatización del Workflow**

GitHub Projects con automatización:

```
┌─────────┬───────────┬─────────────┬────────────┬──────┐
│ Backlog │ Todo (H1) │ In Progress │ In Review  │ Done │
├─────────┼───────────┼─────────────┼────────────┼──────┤
│ Issue 10│ Issue 2   │  Issue 3    │   PR 5     │ #1   │
│ Issue 11│ Issue 4   │  Issue 6    │   PR 7     │ #2   │
└─────────┴───────────┴─────────────┴────────────┴──────┘

Automatizaciones:
- Issue creado → va a "Backlog"
- Issue asignado con label "hito-1" → "Todo (H1)"
- PR abierto → "In Review" (automático)
- PR merged → "Done" (automático)
```

#### 7. **Sistema de Labels**

Labels configurados para categorización:

| Label             | Uso                | Color           |
| ----------------- | ------------------ | --------------- |
| `bug`             | Errores a corregir | 🔴 Rojo         |
| `enhancement`     | Nuevas features    | 🔵 Azul         |
| `documentation`   | Cambios en docs    | 📘 Azul oscuro  |
| `hito-1`          | Tareas del Hito 1  | 🟡 Amarillo     |
| `priority-high`   | Urgente            | 🔥 Rojo intenso |
| `priority-medium` | Importante         | 🟠 Naranja      |
| `priority-low`    | Puede esperar      | 🟢 Verde        |

### Comparación con Alternativas Descartadas

**Jira** ❌ DESCARTADO

- ❌ Complejidad excesiva (curva de aprendizaje de días)
- ❌ Coste: Plan Standard $7.75/usuario/mes = $38.75/mes
- ❌ Overhead: Diseñado para equipos enterprise (50-500+ personas)
- ❌ Separación de GitHub: Requiere plugin, sincronización no instantánea

**Trello** ❌ DESCARTADO

- ❌ Plan gratuito limitado (10 boards, sin automatizaciones avanzadas)
- ❌ No diseñado para desarrollo (no hay concepto nativo de bug vs feature)
- ❌ No se integra con commits
- ❌ Sincronización manual con GitHub
- ❌ Pérdida de trazabilidad (no hay link directo entre tarjeta y código)

**Azure DevOps** ❌ DESCARTADO

- ❌ Ecosistema diferente (diseñado para stacks Microsoft)
- ❌ Complejidad: Suite completa innecesaria
- ❌ Redundancia: Ya tenemos GitHub para repos y CI/CD

## 3.3 Implementación en el Proyecto

### Ciclo de Vida de un Bug

```
1. DISCOVERY → Bug encontrado durante testing/desarrollo
   ↓
2. REPORTING → Click "New Issue" → Usar "Bug Report" template
   ↓
3. TRIAJE → Evaluar severidad, asignar prioridad y desarrollador
   ↓
4. FIXING → Developer crea rama bugfix/*, hace commits
   ↓
5. REVIEW → Abrir PR con "Closes #N", code review
   ↓
6. VERIFICATION → Merge a main, issue se cierra automáticamente
   ↓
7. CLOSURE → Bug resuelto ✅
```

### Ejemplo Real

```
Bug encontrado: "Jugador atraviesa plataformas al caer"
↓
Issue #8 creado (Bug Report template)
- Descripción detallada
- Pasos para reproducir
- Label: bug, priority-high, hito-1
- Asignado a: Rachid (responsable de físicas)
↓
Rachid crea rama: bugfix/player-fall-through
↓
Rachid hace commits: "fix: add velocity check (#8)"
↓
PR abierto: "Fix player collision (Closes #8)"
- Descripción de cambios
- Testing realizado
- Screenshots si aplica
↓
Code review por Ángel → Aprobado
↓
Merge a main
↓
Issue #8 cerrado automáticamente ✅
```

---

# 4. Contribuciones a Otros Proyectos

## 4.1 Metodología de Evaluación

Como parte del Hito 1, analizaremos la versión jugable de otro grupo de la asignatura DCA y aportaremos feedback constructivo mediante tickets en su repositorio.

### Proceso de Análisis

**Pasos:**

1. Acceso al proyecto asignado por el profesor
2. Clonar repositorio y compilar siguiendo sus instrucciones
3. Evaluación sistemática:
   - Probar todas las funcionalidades documentadas
   - Buscar bugs o comportamientos inesperados
   - Evaluar usabilidad y experiencia de usuario
   - Revisar calidad del código (si es accesible)
4. Creación de issues en SU repositorio

### Criterios de Evaluación

| Aspecto           | Qué evaluar                        |
| ----------------- | ---------------------------------- |
| **Funcionalidad** | ¿Funciona como describe el README? |
| **Bugs**          | ¿Hay errores que impiden jugar?    |
| **Usabilidad**    | ¿Es fácil entender cómo jugar?     |
| **Rendimiento**   | ¿Hay lag o problemas de FPS?       |
| **Documentación** | ¿Es clara y completa?              |
| **Compilación**   | ¿Se compila fácilmente?            |

## 4.2 Formato de Issues a Crear

### Template para Bug Reports

```markdown
**Título:** [BUG] Descripción corta del problema

**Descripción:**
[Explicación clara del bug encontrado]

**Pasos para reproducir:**

1. [Paso 1]
2. [Paso 2]
3. [Observar error]

**Comportamiento esperado:**
[Qué debería ocurrir]

**Comportamiento actual:**
[Qué ocurre realmente]

**Entorno:**

- OS: [Linux/Windows]
- Versión: [v0.x.x]

**Severidad:**

- [ ] Critical (impide jugar)
- [ ] High (afecta significativamente)
- [ ] Medium (molesto pero jugable)
- [ ] Low (cosmético)

---

_Issue creado por Chaos Crew en evaluación de Hito 1_
```

### Template para Mejoras

```markdown
**Título:** [ENHANCEMENT] Sugerencia de mejora

**Descripción:**
[Qué se podría mejorar y por qué]

**Beneficio:**
[Cómo mejoraría el juego/proyecto]

**Prioridad sugerida:**

- [ ] High - Impacto significativo
- [ ] Medium - Mejora notable
- [ ] Low - Nice to have

---

_Sugerencia del equipo Chaos Crew - Evaluación Hito 1_
```

## 4.3 Compromiso

**Mínimo:** 5 issues de calidad

- Al menos 2 bugs reproducibles
- Al menos 2 sugerencias de mejora
- Al menos 1 sobre documentación

**Distribución:** Cada miembro crea al menos 1 issue

## 4.4 Registro de Contribuciones

**Proyecto evaluado:** [Será completado cuando se asigne]  
**Fecha de evaluación:** [Fecha]

| #   | Tipo        | Título | Autor  | Link   | Severidad |
| --- | ----------- | ------ | ------ | ------ | --------- |
| 1   | Bug         | ...    | Erardo | [link] | High      |
| 2   | Enhancement | ...    | Ángel  | [link] | Medium    |
| ... | ...         | ...    | ...    | ...    | ...       |

---

# 5. Estado Actual del Proyecto

## 5.1 Funcionalidades Implementadas

### ✅ Sistema de Gestión de Estados

- Enum `GameScreen` con estados: GAMEPLAY, GAME_OVER
- Transiciones limpias entre estados

### ✅ Sistema de Movimiento de Jugadores

- Estructura `Player` con posición, velocidad, color
- Input handling para 2 jugadores simultáneos (WASD + Flechas)
- Movimiento horizontal suave (4.5 units/frame)
- Sistema de salto con física (impulso -12.0, gravedad 0.6)

### ✅ Sistema de Físicas

- Gravedad constante aplicada cuando no están en suelo
- Acumulación de velocidad vertical
- Estado `isGrounded` para control de salto
- Prevención de double jump

### ✅ Sistema de Colisiones

- Detección AABB (Axis-Aligned Bounding Box)
- Colisión jugador-plataforma
- Resolución vertical (reposicionamiento)
- Detección de colisión con objetivo

### ✅ Nivel Básico Funcional

- Plataforma principal (suelo): 1280x50 píxeles
- 4 plataformas intermedias de diferentes alturas
- Zona de salida: Rectángulo verde 80x100 píxeles

### ✅ Pantallas Implementadas

**Gameplay:**

- Renderizado de plataformas, jugadores y salida
- HUD con instrucciones
- Contador de FPS

**Game Over:**

- Mensaje diferenciado (victoria/derrota)
- Opción de reiniciar con ENTER

### ✅ Condiciones de Victoria/Derrota

**Victoria:** ✅ AMBOS jugadores en zona de salida  
**Derrota:** ❌ CUALQUIER jugador cae fuera de pantalla  
**Reinicio:** ENTER resetea todo y vuelve a GAMEPLAY

## 5.2 Métricas del Proyecto

| Métrica              | Valor                    |
| -------------------- | ------------------------ |
| **Líneas de código** | ~320 (main.cpp)          |
| **Archivos fuente**  | 1 (monolítico en Hito 1) |
| **FPS objetivo**     | 60                       |
| **Resolución**       | 1280x720                 |
| **Duración nivel**   | 30-60 segundos           |

**Nota:** En Hito 2 se modularizará el código en múltiples archivos.

## 5.3 Estructura Actual del Código

```
chaos-crew-dca/
├── src/
│   └── main.cpp              # Código principal (320 líneas)
├── docs/
│   ├── HITO1.md              # Este documento
│   ├── GDD.md                # Game Design Document completo
│   └── images/               # Screenshots
├── bin/                      # Ejecutables (NO en Git)
├── releases/                 # Packages de releases
├── .gitignore
├── README.md
└── build.sh                  # Script de compilación
```

---

# 6. Instrucciones de Compilación

## 6.1 Requisitos

### Dependencias del Sistema

**Linux (Ubuntu/Debian):**

```bash
# Instalar herramientas de desarrollo
sudo apt update
sudo apt install build-essential git

# Instalar dependencias de Raylib
sudo apt install libasound2-dev libx11-dev libxrandr-dev \
                 libxi-dev libgl1-mesa-dev libglu1-mesa-dev \
                 libxcursor-dev libxinerama-dev
```

### Raylib 5.0

**Linux:**

```bash
# Clonar e instalar Raylib
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
cd ../..
```

**Verificar instalación:**

```bash
pkg-config --modversion raylib
# Debería mostrar: 5.0 (o similar)
```

## 6.2 Compilación del Proyecto

### Opción 1: Script Automatizado (Recomendado)

```bash
# Clonar repositorio
git clone https://github.com/eap59-ua/chaos-crew-dca.git
cd chaos-crew-dca

# Dar permisos al script
chmod +x build.sh

# Compilar para Linux
./build.sh linux

# Ejecutar
./bin/chaos-crew
```

### Opción 2: Compilación Manual

**Linux:**

```bash
g++ src/main.cpp -o bin/chaos-crew \
    -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
    -std=c++17 -Wall -Wextra -O2

./bin/chaos-crew
```

## 6.3 Solución de Problemas Comunes

**Error: "raylib.h: No such file or directory"**

```bash
# Verificar instalación de Raylib
pkg-config --cflags raylib

# Si falla, reinstalar Raylib
```

**Error: "undefined reference to InitWindow"**

```bash
# Asegurar flags de linkeo correctos
-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

---

# 7. Contribuciones del Equipo

## 7.1 Distribución de Trabajo

| Miembro     | Rol            | Horas | Issues     | PRs | Contribución Principal    |
| ----------- | -------------- | ----- | ---------- | --- | ------------------------- |
| **Erardo**  | Project Lead   | 18h   | #1, #6, #7 | X   | Setup, docs, build system |
| **Ángel**   | Gameplay Dev   | 15h   | #2         | Y   | Player movement, input    |
| **Rachid**  | Systems Dev    | 16h   | #3         | Z   | Physics, collisions       |
| **Youssef** | Level Designer | 12h   | #5         | W   | Level design, testing     |
| **Darío**   | UI/Screens     | 14h   | #4         | V   | Game states, screens      |

**Total:** ~75 horas de trabajo del equipo

## 7.2 Estadísticas del Repositorio

| Métrica             | Valor                 |
| ------------------- | --------------------- |
| **Commits totales** | [Actualizar al final] |
| **Pull Requests**   | [Actualizar al final] |
| **Issues creados**  | [Actualizar al final] |
| **Issues cerrados** | [Actualizar al final] |
| **Code reviews**    | [Actualizar al final] |

**Nota:** Estas estadísticas se actualizarán en la entrega final.

---

# 8. Próximos Pasos (Hito 2)

## 8.1 Objetivos Principales

- 📦 **Refactorización:** Modularizar código en múltiples archivos
- 🎮 **Multijugador:** Soporte 3-5 jugadores con gamepads
- ⚠️ **Trampas dinámicas:** Plataformas desaparecientes, pinchos
- 🤝 **Mecánicas cooperativas:** Botones múltiples, puertas sincronizadas
- 🎨 **Arte:** Sprites pixel art
- 🎵 **Audio:** Música y efectos de sonido
- ✅ **Testing:** Suite de 10+ tests automatizados
- 🔄 **CI/CD:** GitHub Actions

---

# 9. Referencias

## 9.1 Documentación Técnica

- **Raylib:** https://www.raylib.com/
- **Raylib Cheatsheet:** https://www.raylib.com/cheatsheet/cheatsheet.html
- **GitHub Flow:** https://docs.github.com/en/get-started/quickstart/github-flow
- **Conventional Commits:** https://www.conventionalcommits.org/

## 9.2 Juegos de Inspiración

- **Level Devil:** https://www.crazygames.com/game/level-devil
- **Pico Park:** https://store.steampowered.com/app/1509960/PICO_PARK/

---

# 10. Conclusiones

## 10.1 Logros del Hito 1

✅ **Requisitos cumplidos:**

- Juego funcional con 2 pantallas (Gameplay + Game Over)
- Mecánica principal de plataformas cooperativas implementada
- Bucle de juego completo (jugar → ganar/perder → reiniciar)
- 2 jugadores simultáneos controlables
- Gráficos con figuras geométricas básicas
- GitHub Flow implementado con protecciones
- GitHub Issues configurado con templates
- Versionado semántico (v0.1.0-alpha)
- Documentación completa

✅ **Calidad técnica:**

- 0 crashes (100% estabilidad)
- 60 FPS constantes
- Código funcional y compilable
- GitHub Flow aplicado correctamente
- Code reviews en todos los PRs

## 10.2 Lecciones Aprendidas

**Desarrollo colaborativo:**

- GitHub Flow es efectivo para equipos pequeños
- Code reviews mejoran calidad Y conocimiento compartido
- Comunicación diaria (standups) crucial para coordinación
- Protección de main previene errores costosos

**Desarrollo de videojuegos:**

- Gameplay simple pero bien ejecutado > features complejas a medias
- Testing constante esencial
- Cooperación genuina difícil de diseñar pero satisfactoria

**Mejoras para Hito 2:**

- Modularizar código desde el inicio
- Testing automatizado desde el principio
- Documentar decisiones técnicas en tiempo real

---

**Versión:** 1.0  
**Fecha:** 29 de octubre de 2025  
**Equipo:** Chaos Crew  
**Repositorio:** https://github.com/eap59-ua/chaos-crew-dca  
**Universidad de Alicante - Desarrollo Colaborativo de Aplicaciones**

---

**FIN DEL DOCUMENTO HITO1.md**
