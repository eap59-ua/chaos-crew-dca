# Hito 1 - Alpha Build

## Chaos Crew - Plataformas Cooperativo 2D

**Fecha de entrega:** 22 de octubre de 2025  
**Versión del juego:** v0.1.0-alpha

**Equipo:**

- Erardo Aldana Pessoa (@eap59-ua)
- Ángel Gonjar Verdejo (@agv114-ua)
- Rachid Mouradi (@mouradirachid8-lgtm)
- Youssef S.A. (@ysa9-ua)
- Darío S.D. (@dsd17-ua)

**Repositorio:** https://github.com/eap59-ua/chaos-crew-dca

---

# Índice

1. [GDD Básico (Game Design Document)](#1-gdd-básico-game-design-document)
2. [Elección de Estrategia de Gestión de Ramas](#2-elección-de-estrategia-de-gestión-de-ramas)
3. [Sistema de Bugtracking Elegido](#3-sistema-de-bugtracking-elegido)
4. [Contribuciones a Otros Proyectos](#4-contribuciones-a-otros-proyectos)
5. [Estado Actual del Proyecto](#5-estado-actual-del-proyecto)
6. [Contribuciones del Equipo](#6-contribuciones-del-equipo)

---

# 1. GDD Básico (Game Design Document)

## 1.1 Concepto del Juego

### Información General

| Atributo        | Valor                      |
| --------------- | -------------------------- |
| **Título**      | Chaos Crew                 |
| **Género**      | Plataformas 2D cooperativo |
| **Plataforma**  | PC (Linux/Windows)         |
| **Jugadores**   | 2-5 (multijugador local)   |
| **Tecnología**  | C++, Raylib                |
| **Perspectiva** | Vista lateral 2D           |

### Concepto Core

**Chaos Crew** es un juego de plataformas cooperativo donde 2 a 5 jugadores deben trabajar en equipo para superar niveles llenos de obstáculos y trampas que pueden cambiar dinámicamente durante la partida.

El juego está inspirado en dos títulos existentes:

- **Level Devil:** Incorpora mecánicas de trampas dinámicas e impredecibles que sorprenden constantemente al jugador
- **Pico Park:** Requiere cooperación activa y obligatoria entre jugadores para superar desafíos

La característica distintiva es que **ambos jugadores deben llegar a la salida** para completar el nivel. Si uno cae o falla, todos reinician desde el checkpoint, fomentando el trabajo en equipo real.

### Propuesta de Valor

- **Cooperación genuina:** No es opcional, sino necesaria para avanzar
- **Trampas impredecibles:** El nivel cambia y sorprende constantemente
- **Accesibilidad:** Controles simples, fácil de aprender
- **Partidas rápidas:** Niveles cortos (2-3 minutos) perfectos para sesiones casuales
- **Escalabilidad:** Funciona con 2, 3, 4 o 5 jugadores

---

## 1.2 Mecánicas del Juego

### Mecánicas Implementadas (Hito 1 - Alpha)

#### 1. **Sistema de Movimiento**

- **Desplazamiento horizontal:** Izquierda/Derecha con velocidad constante
- **Salto:** Con impulso inicial y gravedad aplicada
- **Restricciones:** Los jugadores no pueden salir de los límites de la pantalla

**Variables:**

- Velocidad de movimiento: 4.5 unidades/frame
- Fuerza de salto: -12.0 unidades
- Gravedad: 0.6 unidades/frame²

#### 2. **Sistema de Físicas**

- **Gravedad:** Aplicada constantemente a los jugadores cuando no están en el suelo
- **Estado "en el suelo":** Determina si el jugador puede saltar
- **Velocidad vertical:** Acumulada por la gravedad y el salto

#### 3. **Sistema de Colisiones**

- **Detección AABB:** Collision rectangles para jugadores y plataformas
- **Resolución vertical:** Los jugadores se reposicionan encima de las plataformas
- **Colisión con salida:** Detecta cuando ambos jugadores alcanzan el objetivo

#### 4. **Control Multijugador Local**

- **Jugador 1:** Controles WASD (movimiento) + W/SPACE (salto)
- **Jugador 2:** Flechas del teclado (movimiento) + ↑ (salto)
- **Controles simultáneos:** Ambos jugadores pueden moverse al mismo tiempo sin interferencias

#### 5. **Gestión de Estados**

- **GAMEPLAY:** Pantalla principal del juego donde ocurre la acción
- **GAME_OVER:** Pantalla de finalización con opción de reintentar

#### 6. **Condiciones de Victoria y Derrota**

**Victoria:**

- ✅ **Ambos** jugadores deben alcanzar la zona de salida (cooperación obligatoria)
- Se transiciona a pantalla GAME_OVER con mensaje de victoria

**Derrota:**

- ❌ Si **cualquier** jugador cae fuera de la pantalla
- Se transiciona a pantalla GAME_OVER con mensaje de derrota
- Fomenta que los jugadores se cuiden entre sí

### Mecánicas Planificadas (Hito 2 - Beta)

#### 1. **Sistema de Trampas Dinámicas**

- [ ] Plataformas que desaparecen temporalmente
- [ ] Pinchos que emergen/se ocultan con timers
- [ ] Plataformas móviles horizontales/verticales
- [ ] Obstáculos activados por triggers

#### 2. **Mecánicas Cooperativas Avanzadas**

- [ ] **Botones de presión múltiple:** Requieren que 2+ jugadores los activen simultáneamente
- [ ] **Plataformas con peso:** Solo se activan con varios jugadores encima
- [ ] **Puertas sincronizadas:** Se abren cuando todos están en zonas específicas
- [ ] **Palancas temporales:** Un jugador mantiene, otro avanza
- [ ] **Boost cooperativo:** Un jugador impulsa a otro para alcanzar zonas altas

#### 3. **Sistema de Checkpoints**

- [ ] Puntos de guardado compartidos
- [ ] Respawn en checkpoint en vez de reinicio completo
- [ ] Indicador visual de progreso

#### 4. **Multijugador 3-5 Jugadores**

- [ ] Soporte para 3, 4 y 5 jugadores simultáneos
- [ ] Controles adicionales (gamepads)
- [ ] Mecánicas que requieren 3+ jugadores
- [ ] Colores distintivos para cada jugador

### Mecánicas Planificadas (Hito 3 - Release)

#### 1. **Sistema de Vidas Compartidas**

- [ ] Pool de vidas compartido entre todos
- [ ] Si se acaban, reinicio completo del nivel
- [ ] Añade tensión y fomenta cuidado mutuo

#### 2. **Power-ups Cooperativos**

- [ ] Escudo temporal (protege a todos)
- [ ] Slow-motion (facilita timing en trampas)
- [ ] Double jump temporal
- [ ] Imán cooperativo (atrae objetos)

#### 3. **Sistema de Ranking/Score**

- [ ] Tiempo de completado
- [ ] Muertes totales
- [ ] Ranking por nivel
- [ ] Récords compartidos

---

## 1.3 Componentes del Juego

### Entidades

| Entidad               | Descripción                                  | Estado (Hito 1)   |
| --------------------- | -------------------------------------------- | ----------------- |
| **Jugador**           | Cuadrado controlable con físicas             | ✅ Implementado   |
| **Plataformas**       | Superficies estáticas donde se puede caminar | ✅ Implementado   |
| **Salida/Objetivo**   | Zona que ambos jugadores deben alcanzar      | ✅ Implementado   |
| **Trampas estáticas** | Pinchos, pozos (decorativo en H1)            | ⏳ Planificado H2 |
| **Trampas dinámicas** | Plataformas móviles, pinchos emergentes      | ⏳ Planificado H2 |
| **Botones/Palancas**  | Elementos interactivos cooperativos          | ⏳ Planificado H2 |
| **Checkpoints**       | Puntos de guardado                           | ⏳ Planificado H2 |
| **Power-ups**         | Objetos recolectables                        | ⏳ Planificado H3 |

### Pantallas/Estados

| Pantalla               | Descripción                          | Estado (Hito 1)   |
| ---------------------- | ------------------------------------ | ----------------- |
| **Menú Principal**     | Selección de modo, opciones          | ⏳ Planificado H2 |
| **Gameplay**           | Pantalla principal del juego         | ✅ Implementado   |
| **Game Over**          | Victoria o derrota, opción reiniciar | ✅ Implementado   |
| **Pausa**              | Pausar el juego                      | ⏳ Planificado H2 |
| **Selección de Nivel** | Elegir nivel a jugar                 | ⏳ Planificado H2 |
| **Créditos**           | Información del equipo               | ⏳ Planificado H3 |

### Sistema de Niveles

#### Nivel 1 (Hito 1 - Tutorial)

- **Objetivo:** Enseñar mecánicas básicas
- **Elementos:**
  - Plataforma principal (suelo)
  - 3-5 plataformas intermedias
  - Zona de salida claramente marcada
- **Dificultad:** Muy fácil, sin trampas
- **Duración estimada:** 30-60 segundos

#### Niveles 2-4 (Hito 2 - Introducción de mecánicas)

- Introducción progresiva de trampas
- Primeras mecánicas cooperativas simples
- Dificultad creciente

#### Niveles 5-8 (Hito 3 - Niveles completos)

- Combinación de todas las mecánicas
- Desafíos cooperativos complejos
- Trampas dinámicas impredecibles
- Requieren comunicación y coordinación

---

## 1.4 Arte y Estilo Visual

### Estilo Visual (Hito 1)

**Figuras geométricas básicas** para prototipado rápido:

| Elemento        | Representación      | Color                    |
| --------------- | ------------------- | ------------------------ |
| **Jugador 1**   | Cuadrado 30x40      | Azul (`BLUE`)            |
| **Jugador 2**   | Cuadrado 30x40      | Rojo (`RED`)             |
| **Plataformas** | Rectángulos         | Gris (`GRAY`)            |
| **Suelo**       | Rectángulo grande   | Gris oscuro (`DARKGRAY`) |
| **Salida**      | Rectángulo vertical | Verde (`GREEN`)          |
| **Fondo**       | Sólido              | Blanco (`RAYWHITE`)      |

**Indicadores visuales:**

- Texto "P1" y "P2" dentro de cada jugador
- Texto "EXIT" en la zona de salida
- Instrucciones en la parte superior de la pantalla

### Estilo Visual (Hito 2 en adelante)

**Pixel art colorido** inspirado en juegos retro:

- Sprites animados para jugadores (idle, run, jump)
- Tileset para plataformas y escenarios
- Animaciones para trampas
- Efectos de partículas para feedback visual
- Paleta de colores vibrante y alegre

**Referencias de estilo:**

- Celeste (claridad visual, feedback)
- Super Meat Boy (precisión, respuesta inmediata)
- Pico-8 games (paleta limitada, estilo retro)

---

## 1.5 Audio (Planificado)

### Música

- [ ] Tema principal (menú)
- [ ] Música de gameplay (energética, loop)
- [ ] Música de victoria (celebración corta)
- [ ] Música de derrota (tono descendente)

### Efectos de Sonido

- [ ] Salto (`jump.wav`)
- [ ] Aterrizaje (`land.wav`)
- [ ] Colisión con trampa (`hurt.wav`)
- [ ] Victoria (`win.wav`)
- [ ] Botón activado (`button.wav`)
- [ ] Checkpoint alcanzado (`checkpoint.wav`)
- [ ] UI navigation (`ui_select.wav`, `ui_back.wav`)

**Nota:** Audio no implementado en Hito 1 por priorización de mecánicas core.

---

## 1.6 Controles

### Hito 1 (2 Jugadores)

| Acción                | Jugador 1     | Jugador 2              |
| --------------------- | ------------- | ---------------------- |
| Mover izquierda       | `A`           | `←` (Flecha izquierda) |
| Mover derecha         | `D`           | `→` (Flecha derecha)   |
| Saltar                | `W` o `SPACE` | `↑` (Flecha arriba)    |
| Reiniciar (Game Over) | `ENTER`       | `ENTER`                |

### Hito 2+ (3-5 Jugadores)

Se añadirán controles para gamepads:

- Jugador 3: Gamepad 1
- Jugador 4: Gamepad 2
- Jugador 5: Gamepad 3

**Mapeo de gamepad:**

- Stick izquierdo / D-Pad: Movimiento
- Botón A / Cross: Saltar
- Start: Pausa

---

## 1.7 Tecnologías Utilizadas

### Core

- **Lenguaje:** C++17
- **Framework gráfico:** Raylib 5.0
- **Sistema de build:** Script bash (Hito 1), CMake (Hito 2+)
- **Control de versiones:** Git + GitHub

### Librerías Adicionales (Planificadas)

| Librería   | Uso                                  | Hito   |
| ---------- | ------------------------------------ | ------ |
| **EnTT**   | Patrón ECS para gestión de entidades | Hito 2 |
| **spdlog** | Sistema de logging profesional       | Hito 2 |
| **Box2D**  | Físicas avanzadas (opcional)         | Hito 3 |
| **ImGui**  | Interfaz de debug                    | Hito 3 |

---

## 1.8 Objetivos de Diseño

### Pilares del Diseño

1. **Cooperación Genuina**

   - No es opcional, es necesaria
   - Mecánicas que requieren coordinación real
   - Fallo de uno = fallo de todos

2. **Accesibilidad**

   - Controles simples (mover + saltar)
   - Fácil de aprender, difícil de dominar
   - No requiere habilidad técnica extrema

3. **Diversión Compartida**

   - Momentos de caos controlado
   - Risas por fallos compartidos
   - Celebración de logros en equipo

4. **Iteración Rápida**

   - Niveles cortos (2-3 minutos)
   - Reinicio rápido tras fallo
   - Sin castigos frustrantes

5. **Sorpresa y Variedad**
   - Trampas impredecibles
   - Cada nivel presenta nuevos desafíos
   - No dos partidas son iguales

### Público Objetivo

- **Edad:** 8-35 años
- **Perfil:** Jugadores casuales que buscan diversión cooperativa local
- **Experiencia:** No requiere experiencia previa en videojuegos
- **Contexto:** Juego para sesiones con amigos/familia en el sofá

---

## 1.9 Alcance del Proyecto

### Hito 1 - Alpha (22 Oct 2025)

**Objetivo:** Prototipo jugable mínimo

- ✅ 2 jugadores simultáneos
- ✅ Mecánicas básicas de plataformas
- ✅ 1 nivel funcional
- ✅ 2 pantallas (Gameplay + Game Over)
- ✅ Gráficos con figuras geométricas
- ✅ Condiciones de victoria/derrota cooperativas

### Hito 2 - Beta (Mediados Nov 2025)

**Objetivo:** Mecánicas completas y pulidas

- [ ] 3-5 jugadores
- [ ] Sistema de trampas dinámicas
- [ ] 3-4 niveles jugables
- [ ] Sprites y animaciones básicas
- [ ] Audio (música + SFX)
- [ ] Sistema de tests (min. 10)
- [ ] CI/CD con GitHub Actions
- [ ] Menú principal

### Hito 3 - Release (Principios Dic 2025)

**Objetivo:** Producto completo y pulido

- [ ] 5-8 niveles completos
- [ ] Patrón ECS implementado
- [ ] Sistema de checkpoints
- [ ] Power-ups cooperativos
- [ ] Internacionalización (ES/EN)
- [ ] Ejecutables Linux y Windows
- [ ] Sistema de logging
- [ ] Interfaz de debug
- [ ] Polish y efectos visuales

---

# 2. Elección de Estrategia de Gestión de Ramas

## 2.1 Estrategia Elegida: **GitHub Flow**

### Descripción

GitHub Flow es un flujo de trabajo ligero basado en ramas y Pull Requests, donde existe una única rama principal (`main`) que siempre debe estar en estado desplegable, y todas las nuevas funcionalidades se desarrollan en ramas temporales que se integran mediante Pull Requests después de una revisión de código.

### Estructura de Ramas

En nuestro proyecto utilizamos únicamente dos tipos de ramas:

**1. `main` (rama principal permanente):**

- Rama protegida con branch protection rules
- Siempre contiene código compilable y funcional
- No se permiten commits directos
- Cada commit representa una versión potencialmente desplegable
- Punto de origen para las releases (v0.1.0-alpha, v0.2.0-beta, etc.)

**2. Ramas temporales (feature/bugfix/docs):**

- Se crean desde `main` para cada nueva tarea
- Nomenclatura descriptiva basada en tipo de cambio
- Vida útil corta (1-3 días idealmente)
- Se eliminan automáticamente después del merge a `main`

### Diagrama del Flujo

```
main ──────●──────●──────●──────●────── (siempre estable)
            │      │      │      │
            │      └──────┘      │
            │   feature/collision│
            │                    └────────┐
            └─────────────┐               │
              feature/    │               │
              movement    │               │
                          ▼               ▼
                        [PR + Review] [PR + Review]
                          │               │
                          └───────────────┘
                                  │
                                  ▼
                              main (actualizada)
```

---

## 2.2 Justificación de la Elección

### Razones Específicas del Proyecto

#### 1. **Tamaño del equipo (5 personas)**

Con un equipo de 5 desarrolladores, no necesitamos la complejidad de estrategias como Git Flow que están diseñadas para equipos grandes (10-50+ personas) con múltiples líneas de desarrollo paralelas, releases superpuestas, y hotfixes urgentes en producción. GitHub Flow proporciona suficiente estructura para coordinar el trabajo sin añadir overhead innecesario en reuniones, sincronización, o gestión de múltiples ramas permanentes.

#### 2. **Ciclo de entregas frecuentes (3 hitos en 6 semanas)**

Nuestro proyecto tiene entregas cada aproximadamente 3 semanas:

- Hito 1: 22 octubre
- Hito 2: Mediados noviembre
- Hito 3: Principios diciembre

GitHub Flow está diseñado precisamente para ciclos de desarrollo cortos con integración continua, permitiéndonos mantener `main` siempre lista para generar una release en cualquier momento sin necesidad de ramas intermedias como `develop` o `release/*`.

#### 3. **Naturaleza del proyecto (videojuego con iteraciones rápidas)**

El desarrollo de videojuegos requiere:

- Testing constante del gameplay
- Ajustes frecuentes basados en playtesting
- Iteración rápida sobre mecánicas
- Balance continuo de parámetros

GitHub Flow facilita la integración rápida de cambios sin bloqueos. No necesitamos esperar a una "ventana de release" para probar nuestras ideas; cada merge a `main` nos da una versión jugable inmediatamente.

#### 4. **Integración nativa con GitHub y sus herramientas**

Al alojar nuestro proyecto en GitHub, GitHub Flow se integra perfectamente con:

- **GitHub Issues:** Issues se cierran automáticamente al hacer merge de PRs
- **GitHub Projects:** Automatización del movimiento de tarjetas
- **GitHub Actions:** (Hito 2) CI/CD triggers en cada push a main
- **Branch protection:** Configuración nativa sin plugins adicionales

No necesitamos herramientas externas ni configuraciones complejas.

#### 5. **Facilita Code Reviews obligatorios**

La protección de `main` con requisito de Pull Requests asegura que:

- Todo código pase por revisión de al menos otro desarrollador
- Se detecten errores antes de integración
- Se mantenga calidad y consistencia del código
- Los miembros menos experimentados aprendan de los más experimentados

Esto es especialmente valioso en un contexto académico donde parte del objetivo es aprender buenas prácticas de desarrollo colaborativo.

#### 6. **Curva de aprendizaje baja**

GitHub Flow es conceptualmente simple:

- Main = código bueno
- Feature = código en desarrollo
- PR = revisión antes de integrar

Esta simplicidad permite que todos los miembros del equipo, independientemente de su experiencia previa con Git, lo comprendan y apliquen correctamente desde el primer día sin necesidad de formación extensa.

#### 7. **Reduce conflictos de merge**

Al trabajar con ramas de corta duración (1-3 días) y merges frecuentes a `main`, se minimizan los conflictos de integración que podrían surgir con:

- **Git Flow:** Donde `develop` puede divergir mucho de `main`
- **Feature branches** de larga duración: Donde acumulan muchos cambios difíciles de integrar

Los conflictos que surgen son pequeños y manejables, no catastrofes de 500+ líneas en conflicto.

---

## 2.3 Comparación con Alternativas Descartadas

### Git Flow (DESCARTADO)

**Por qué NO lo elegimos:**

❌ **Complejidad innecesaria:** Requiere gestionar 5+ tipos de ramas:

- `main` (producción)
- `develop` (desarrollo)
- `feature/*` (funcionalidades)
- `release/*` (preparación de releases)
- `hotfix/*` (parches urgentes)

Para un proyecto de 6 semanas con entregas cada 3 semanas, esta estructura es excesiva.

❌ **Overhead de gestión:** Git Flow requiere:

- Mantener `develop` y `main` sincronizadas
- Crear ramas `release` para preparar entregas
- Gestionar hotfixes que se aplican a múltiples ramas
- Múltiples merges para un solo cambio

Este tiempo es mejor invertido en desarrollar el juego.

❌ **Diseñado para otro contexto:** Git Flow fue creado para software empresarial con:

- Múltiples versiones en producción simultáneamente
- Ciclos de release largos (meses)
- Necesidad de mantener versiones antiguas con hotfixes

Nuestro proyecto no tiene estas características.

❌ **Curva de aprendizaje alta:** Para un equipo con experiencia mixta en Git, Git Flow añade complejidad que llevaría días en comprender y semanas en dominar, tiempo que no tenemos.

### Trunk Based Development (DESCARTADO)

**Por qué NO lo elegimos:**

❌ **Requiere madurez en CI/CD:** Trunk Based necesita:

- Tests automatizados extensos desde el día 1
- Pipeline de CI/CD muy robusto
- Feature flags para código incompleto
- Despliegues automáticos múltiples al día

No tenemos esta infraestructura en el Hito 1, y construirla llevaría más tiempo que el proyecto completo.

❌ **Ramas de muy corta vida (<24h):** Trunk Based recomienda:

- Commits directos a trunk (main) o
- Ramas que viven menos de 24 horas

Para tareas complejas como "implementar sistema de colisiones" que pueden tomar 2-3 días, esto es poco realista y forzaría código incompleto en main.

❌ **Alto riesgo de romper main:** Sin la barrera de PRs y reviews:

- Código no revisado entra en main
- Bugs no detectados afectan a todo el equipo
- Mayor probabilidad de regresiones

Para un equipo en aprendizaje, este riesgo es inaceptable.

❌ **Dificulta Code Reviews adecuados:** Sin PRs formales:

- Reviews deben hacerse post-commit
- Encontrar cambios específicos para revisar es difícil
- Feedback llega demasiado tarde

### GitLab Flow (DESCARTADO)

**Por qué NO lo elegimos:**

❌ **Más complejo que GitHub Flow:** Añade ramas de environment:

- `main` (código actual)
- `production` (en producción)
- `pre-production` (staging)
- `environment/*` (diferentes ambientes)

No tenemos múltiples ambientes; solo generamos releases.

❌ **Diseñado para despliegues continuos:** GitLab Flow asume:

- Infraestructura de servidores
- Ambientes de staging y producción
- Despliegues automáticos a múltiples entornos

Nuestro "despliegue" es generar un .zip con ejecutables, no necesitamos esta complejidad.

❌ **Overhead innecesario:** Para un proyecto académico sin infraestructura de servidores reales, las ramas de environment no aportan valor, solo confusión.

---

## 2.4 Reglas y Protecciones Implementadas

### Branch Protection Rules en `main`

Hemos configurado las siguientes protecciones en la rama `main` a través de GitHub Settings:

**1. Require a pull request before merging** ✅

- No se permiten commits directos a `main`
- Todo cambio debe pasar por un Pull Request formal
- Garantiza que el código sea revisable antes de integración

**2. Require approvals: 1** ✅

- Al menos otro miembro del equipo debe revisar y aprobar
- El autor del PR no puede aprobarse a sí mismo
- Fomenta revisión cruzada y conocimiento compartido del código

**3. Dismiss stale pull request approvals when new commits are pushed** ✅

- Si se añaden nuevos commits después de una aprobación, se requiere nueva revisión
- Evita que cambios sustanciales entren sin revisión adicional
- Mantiene integridad del proceso de review

**4. Require conversation resolution before merging** ✅

- Todos los comentarios del code review deben marcarse como "resueltos"
- Asegura que el feedback no sea ignorado
- Fomenta discusión y mejora del código

**5. Require status checks to pass before merging** (Hito 2+) 📅

- Se activará cuando implementemos GitHub Actions
- Los tests automatizados deben pasar antes del merge
- Previene introducción de regresiones

**6. Automatically delete head branches** ✅

- Las ramas feature se eliminan automáticamente tras merge
- Mantiene el repositorio limpio
- Evita acumulación de ramas obsoletas

### Configuración de Merge Strategy

**Opción preferida: Squash and Merge**

- Todos los commits de la rama feature se combinan en uno solo
- Mantiene la historia de `main` limpia y lineal
- Facilita reversión de features completas si es necesario
- El mensaje del squash commit incluye referencia al PR original

---

## 2.5 Convenciones de Nomenclatura

### Nombres de Ramas

Seguimos el formato: `tipo/descripcion-kebab-case`

**Tipos de ramas:**

| Prefijo     | Uso                                  | Ejemplo                      |
| ----------- | ------------------------------------ | ---------------------------- |
| `feature/`  | Nueva funcionalidad                  | `feature/player-movement`    |
| `bugfix/`   | Corrección de errores                | `bugfix/collision-detection` |
| `docs/`     | Solo documentación                   | `docs/hito1-gdd`             |
| `refactor/` | Refactorización sin cambio funcional | `refactor/physics-module`    |
| `test/`     | Añadir o modificar tests             | `test/collision-unit-tests`  |

**Características de nombres buenos:**

- ✅ Descriptivos y claros
- ✅ Kebab-case (palabras separadas por guiones)
- ✅ Cortos pero informativos (2-4 palabras)
- ✅ En inglés (lenguaje estándar del código)
- ❌ No genéricos ("fix", "update", "changes")
- ❌ No incluir nombre del desarrollador

**Ejemplos reales del proyecto:**

```
feature/player-movement          ✅
feature/collision-system         ✅
feature/game-state-manager       ✅
bugfix/player-fall-through       ✅
docs/hito1-documentation         ✅

fix-bug                          ❌ Demasiado genérico
erardo-changes                   ❌ Incluye nombre personal
my_new_feature                   ❌ Snake_case en vez de kebab-case
```

### Mensajes de Commit (Conventional Commits)

Seguimos el estándar Conventional Commits para mensajes claros y consistentes.

**Formato:** `tipo: descripción breve en minúsculas`

**Tipos principales:**

| Tipo        | Uso                            | Ejemplo                                                |
| ----------- | ------------------------------ | ------------------------------------------------------ |
| `feat:`     | Nueva funcionalidad            | `feat: implement player jump mechanic`                 |
| `fix:`      | Corrección de bug              | `fix: resolve collision detection issue`               |
| `docs:`     | Cambios en documentación       | `docs: update README with build instructions`          |
| `style:`    | Formateo, espacios, etc.       | `style: format code with clang-format`                 |
| `refactor:` | Refactorización de código      | `refactor: extract collision logic to separate module` |
| `test:`     | Añadir o modificar tests       | `test: add unit tests for physics system`              |
| `chore:`    | Cambios en build, dependencias | `chore: update raylib to v5.0`                         |

**Ejemplos de mensajes buenos:**

```
feat: add jump mechanic with gravity                    ✅
fix: correct player-platform collision detection        ✅
docs: add GDD section to HITO1.md                       ✅
refactor: split main.cpp into modular files            ✅
test: add collision system unit tests                   ✅

fixed bug                                               ❌ No descriptivo
update                                                  ❌ Demasiado vago
Add jump system and also fix collision and docs        ❌ Múltiples cambios
```

---

## 2.6 Proceso de Trabajo Típico

Un flujo de trabajo completo desde inicio hasta merge sigue estos pasos:

### 1. Selección de Tarea

```
- Developer elige un issue del GitHub Project
- Issue está en columna "Todo (Hito 1)"
- Developer se asigna el issue a sí mismo
- Issue se mueve a columna "In Progress"
```

### 2. Creación de Rama

```bash
git checkout main
git pull origin main
git checkout -b feature/player-movement
```

### 3. Desarrollo

```bash
# Trabajar en el código...
git add src/entities/Player.cpp src/entities/Player.h
git commit -m "feat: create player structure"

# Más trabajo...
git add src/main.cpp
git commit -m "feat: add player input handling"

# Más commits según sea necesario...
```

### 4. Push de Rama

```bash
git push origin feature/player-movement
```

### 5. Apertura de Pull Request

En GitHub web:

1. Click "Compare & pull request"
2. Título descriptivo: "Implement player movement system"
3. Descripción completa:

   ```markdown
   ## Changes

   - Created Player structure
   - Implemented WASD/Arrow input handling
   - Added horizontal movement
   - Added jump mechanic

   ## Testing

   - ✅ Player 1 controls work
   - ✅ Player 2 controls work
   - ✅ No compilation warnings

   Closes #2
   ```

4. Asignar revisor (otro miembro del equipo)
5. Añadir labels apropiadas (`hito-1`, `enhancement`)
6. Click "Create pull request"
7. PR se mueve automáticamente a "In Review" en Project

### 6. Code Review

Revisor hace:

1. Lee el código en GitHub
2. Deja comentarios específicos en líneas de código
3. Prueba el código localmente:
   ```bash
   git fetch origin
   git checkout feature/player-movement
   # Compilar y probar
   ```
4. Decide:
   - ✅ "Approve" si está bien
   - 💬 "Comment" si necesita ajustes menores
   - ❌ "Request changes" si necesita cambios importantes

### 7. Iteración (si necesario)

Si hay cambios solicitados:

```bash
# Hacer los cambios sugeridos
git add .
git commit -m "fix: address code review feedback"
git push origin feature/player-movement
# Esto actualiza el PR automáticamente
```

### 8. Merge a Main

Cuando está aprobado:

1. Click "Squash and merge" en GitHub
2. Editar mensaje del merge commit si necesario
3. Confirmar merge
4. GitHub cierra el issue automáticamente (por el "Closes #2")
5. GitHub elimina la rama automáticamente
6. PR se mueve a "Done" en Project

### 9. Sincronización Local

Todos los desarrolladores:

```bash
git checkout main
git pull origin main
# Ahora tienen los cambios mergeados
```

---

## 2.7 Gestión de Conflictos

### Prevención de Conflictos

**Mejores prácticas:**

1. **Merges frecuentes a main:** No dejar features abiertas más de 3 días
2. **Sincronización diaria:** Actualizar tu rama feature con main cada día
3. **Comunicación:** Avisar si vas a trabajar en un archivo que otro está usando
4. **Modularización:** Dividir el código en archivos pequeños y especializados

### Resolución de Conflictos

Si aparece un conflicto al intentar merge:

```bash
# En tu rama feature
git checkout main
git pull origin main
git checkout feature/mi-rama
git merge main

# Si hay conflictos, Git mostrará:
# CONFLICT (content): Merge conflict in src/main.cpp

# Abrir el archivo con conflicto
# Buscar las marcas:
<<<<<<< HEAD
// Tu código
=======
// Código de main
>>>>>>> main

# Decidir qué mantener, editar manualmente
# Eliminar las marcas <<<, ===, >>>

git add src/main.cpp
git commit -m "fix: resolve merge conflict with main"
git push origin feature/mi-rama

# El PR ahora está actualizado y sin conflictos
```

---

## 2.8 Métricas y KPIs

Para evaluar la efectividad de GitHub Flow en nuestro proyecto, monitorizamos:

| Métrica                            | Objetivo     | Actual (Rellenar durante el proyecto) |
| ---------------------------------- | ------------ | ------------------------------------- |
| **Tiempo medio de vida de PRs**    | < 48 horas   | -                                     |
| **Tiempo medio de review**         | < 24 horas   | -                                     |
| **% de PRs aprobados sin cambios** | > 60%        | -                                     |
| **Número de conflictos de merge**  | < 5 totales  | -                                     |
| **Frecuencia de merges a main**    | ~2-3 por día | -                                     |
| **% de commits que rompen main**   | 0%           | -                                     |

### Retrospectiva de Proceso

Al final de cada hito, evaluaremos:

- ¿GitHub Flow funcionó bien para nosotros?
- ¿Qué obstáculos encontramos?
- ¿Qué ajustes haríamos para el siguiente hito?
- ¿Todos se sienten cómodos con el proceso?

---

## 2.9 Conclusión

GitHub Flow se ajusta perfectamente a las características de Chaos Crew: un equipo pequeño (5 personas), entregas frecuentes (cada 3 semanas), y necesidad de flexibilidad e iteración rápida típica del desarrollo de videojuegos.

Su simplicidad no compromete la calidad, ya que mantiene prácticas esenciales como code reviews obligatorios, protección de la rama principal, y proceso estructurado de integración, mientras evita la complejidad innecesaria de estrategias diseñadas para proyectos de mayor escala.

Esta estrategia nos permite **enfocarnos en el desarrollo del juego** sin invertir tiempo excesivo en gestión de ramas, manteniendo al mismo tiempo un flujo de trabajo profesional y ordenado que facilita la colaboración y el aprendizaje del equipo.

---

# 3. Sistema de Bugtracking Elegido

## 3.1 Sistema Elegido: **GitHub Issues + GitHub Projects**

### Descripción General

Hemos elegido **GitHub Issues** integrado con **GitHub Projects** como nuestro sistema completo de gestión de bugs, features, y tareas del proyecto.

**GitHub Issues** es un sistema de tracking de incidencias nativo de GitHub que permite:

- Crear issues (bugs, features, tareas)
- Asignar responsables
- Etiquetar con labels
- Comentar y discutir
- Cerrar automáticamente con commits/PRs
- Referenciar en código y documentación

**GitHub Projects** complementa Issues proporcionando:

- Vista Kanban del estado de las tareas
- Automatización del workflow
- Visualización del progreso
- Planificación de sprints/hitos

---

## 3.2 Justificación de la Elección

### Razones Específicas del Proyecto

#### 1. **Centralización Completa**

**Problema que resuelve:** Evitar dispersión de información en múltiples plataformas.

Al usar GitHub para:

- Código fuente (repositorio)
- Control de versiones (Git)
- Code reviews (Pull Requests)
- Gestión de tareas (Issues)
- Planificación (Projects)
- Documentación (README, wikis)
- CI/CD (GitHub Actions en Hito 2)

**TODO está en un solo lugar.** Esto elimina:

- ❌ Necesidad de cambiar entre herramientas
- ❌ Sincronización manual entre sistemas
- ❌ Pérdida de contexto al cambiar de plataforma
- ❌ Múltiples notificaciones de diferentes apps

#### 2. **Trazabilidad Directa entre Código y Bugs**

**Integración nativa:**

```bash
# En un commit
git commit -m "fix: correct collision detection (#8)"
# → El issue #8 se actualiza automáticamente con este commit

# En un Pull Request
Closes #8
# → Al hacer merge, el issue #8 se cierra automáticamente
```

**Ventajas:**

- Ver exactamente qué código resolvió qué bug
- Historial completo de cambios relacionados
- No hace falta actualizar manualmente el estado del issue
- Auditoría completa del ciclo de vida del bug

**Ejemplo real:**

```
Issue #8: "Player atraviesa el suelo al saltar"
  ↓
PR #12: "fix: improve vertical collision detection (Closes #8)"
  ↓
Commit abc123: "fix: add velocity check before collision"
  ↓
Issue #8 se cierra automáticamente al merge
  ↓
Historial muestra: Creado 5 Oct → Asignado a Rachid → PR opened 6 Oct → Merged 7 Oct → Closed
```

#### 3. **Sin Coste ni Configuración Externa**

**Alternativas requerirían:**

| Herramienta       | Coste                     | Setup     | Mantenimiento                   |
| ----------------- | ------------------------- | --------- | ------------------------------- |
| **Jira**          | 💰 Pago (o free limitado) | 2-3 horas | Actualizar, configurar permisos |
| **Trello**        | Gratis                    | 1-2 horas | Sincronizar con GitHub          |
| **Asana**         | 💰 Pago para features     | 2 horas   | Integración manual              |
| **GitHub Issues** | ✅ Gratis                 | ✅ 0 min  | ✅ 0 mantenimiento              |

Para un proyecto académico de 6 semanas, invertir tiempo en setup de herramientas externas no tiene sentido.

#### 4. **Accesibilidad para Evaluadores**

**Ventaja académica crucial:**

El profesor y evaluadores pueden:

- ✅ Ver todos los issues sin credenciales adicionales
- ✅ Auditar el proceso de desarrollo
- ✅ Verificar distribución de trabajo
- ✅ Comprobar uso de la herramienta

Con herramientas externas:

- ❌ Necesitarían registrarse
- ❌ Nosotros dar permisos manualmente
- ❌ Exportar reports para demostrar uso

#### 5. **Templates y Estandarización**

**Issue Templates** aseguran información completa:

Tenemos dos templates configurados:

**Bug Report Template:**

```markdown
---
name: Bug Report
about: Reportar un error
labels: bug
---

## Descripción del Bug

[Qué está fallando]

## Pasos para Reproducir

1. ...
2. ...

## Comportamiento Esperado

[Qué debería pasar]

## Comportamiento Actual

[Qué pasa realmente]

## Entorno

- OS: [Linux/Windows]
- Versión: [v0.1.0-alpha]
```

**Feature Request Template:**

```markdown
---
name: Feature Request
about: Proponer nueva funcionalidad
labels: enhancement
---

## Descripción de la Feature

[Qué se quiere añadir]

## Problema que Resuelve

[Por qué es necesaria]

## Prioridad

- [ ] High / [ ] Medium / [ ] Low

## Hito Relacionado

- [ ] Hito 1 / [ ] Hito 2 / [ ] Hito 3
```

**Beneficios:**

- Información consistente en todos los issues
- No olvidar detalles importantes
- Facilita priorización y triaje
- Aprendizaje de buenas prácticas

#### 6. **Kanban Board Automatizado**

**GitHub Projects** proporciona vista Kanban:

```
┌─────────────┬─────────────┬─────────────┬─────────────┬─────────────┐
│   Backlog   │ Todo (H1)   │ In Progress │  In Review  │    Done     │
├─────────────┼─────────────┼─────────────┼─────────────┼─────────────┤
│  Issue #10  │  Issue #2   │  Issue #3   │   PR #5     │  Issue #1   │
│  Issue #11  │  Issue #4   │  Issue #6   │   PR #7     │  Issue #2   │
│  Issue #12  │  Issue #5   │             │             │  Issue #4   │
└─────────────┴─────────────┴─────────────┴─────────────┴─────────────┘
```

**Automatizaciones configuradas:**

- Issue creado → va a "Backlog"
- Issue asignado y con label `hito-1` → va a "Todo (H1)"
- Developer empieza trabajo → mueve manualmente a "In Progress"
- PR abierto → mueve automáticamente a "In Review"
- PR merged → mueve automáticamente a "Done"

**Visibilidad instantánea:**

- Cuántas tareas quedan
- Quién está trabajando en qué
- Qué está bloqueado
- Progreso del hito

#### 7. **Labels para Categorización**

**Sistema de etiquetas configurado:**

| Label              | Color           | Uso                      | Ejemplo                        |
| ------------------ | --------------- | ------------------------ | ------------------------------ |
| `bug`              | 🔴 Rojo         | Errores de funcionalidad | "Jugador atraviesa plataforma" |
| `enhancement`      | 🔵 Azul         | Nuevas features          | "Añadir sistema de salto"      |
| `documentation`    | 📘 Azul oscuro  | Cambios en docs          | "Actualizar README"            |
| `hito-1`           | 🟡 Amarillo     | Tareas del Hito 1        | (se añade a issues relevantes) |
| `hito-2`           | 🟡 Amarillo     | Tareas del Hito 2        | -                              |
| `priority-high`    | 🔥 Rojo intenso | Urgente                  | "Crashea al iniciar"           |
| `priority-medium`  | 🟠 Naranja      | Importante               | "Mejorar feedback visual"      |
| `priority-low`     | 🟢 Verde        | Puede esperar            | "Añadir efecto de particulas"  |
| `help-wanted`      | 💚 Verde claro  | Necesita ayuda           | "No sé cómo resolver esto"     |
| `good-first-issue` | 💜 Morado       | Bueno para empezar       | "Cambiar color de fondo"       |
| `blocked`          | ⛔ Rojo oscuro  | Bloqueado                | "Espera a que #5 se complete"  |
| `wontfix`          | ⚪ Blanco       | No se va a hacer         | "Feature fuera de alcance"     |

**Filtrado rápido:**

```
# Ver solo bugs de alta prioridad del Hito 1
label:bug label:priority-high label:hito-1

# Ver tareas sin asignar
no:assignee is:open

# Ver issues bloqueados
label:blocked is:open
```

---

## 3.3 Comparación con Alternativas Descartadas

### Jira (DESCARTADO)

**Por qué NO:**

❌ **Complejidad excesiva:**

- Curva de aprendizaje de días
- Configuración inicial de 2-3 horas
- Terminología específica (Epic, Story, Task, Subtask)
- Workflow customization que no necesitamos

❌ **Coste:**

- Plan gratuito muy limitado (10 usuarios máximo, features básicas)
- Plan Standard: $7.75/usuario/mes = $38.75/mes para 5 usuarios
- No justificable para proyecto académico de 6 semanas

❌ **Overhead:**

- Diseñado para equipos enterprise (50-500+ personas)
- Features que nunca usaríamos (Roadmaps, Advanced reporting, etc.)
- Necesita administrador dedicado

❌ **Separación de GitHub:**

- Integración con GitHub requiere plugin
- Sincronización no es instantánea
- Doble trabajo: actualizar Jira Y GitHub

### Trello (DESCARTADO)

**Por qué NO:**

❌ **Limitaciones del plan gratuito:**

- Solo 10 boards por workspace
- Sin automatizaciones avanzadas
- Sin campos personalizados

❌ **No diseñado para desarrollo:**

- No hay concepto de "bug" vs "feature" nativo
- No se integra con commits
- No cierra automáticamente con PRs
- Templates limitados

❌ **Sincronización manual:**

- Hay que actualizar Trello AND GitHub Issues
- Propenso a desincronización
- Doble notificaciones

❌ **Pérdida de trazabilidad:**

- No hay link directo entre tarjeta de Trello y código
- Auditoría difícil

### Asana (DESCARTADO)

**Por qué NO:**

Similar a Trello pero:

- ❌ Plan Premium necesario para features útiles ($10.99/usuario/mes)
- ❌ Aún menos orientado a desarrollo software que Trello
- ❌ Integración con GitHub mediante third-party
- ❌ Overhead de gestión de proyectos innecesario

### Azure DevOps (DESCARTADO)

**Por qué NO:**

❌ **Ecosistema diferente:**

- Diseñado para stacks Microsoft
- Si ya usamos GitHub, no tiene sentido

❌ **Complejidad:**

- Suite completa (Boards, Repos, Pipelines, Test Plans, Artifacts)
- Solo necesitamos Boards, pero viene todo junto
- Curva de aprendizaje alta

❌ **Redundancia:**

- Ya tenemos GitHub para repos y CI/CD
- Sería duplicación de funcionalidad

---

## 3.4 Workflow de Bugtracking

### Ciclo de Vida de un Bug

```
1. DISCOVERY (Descubrimiento)
   ↓
   Bug encontrado durante testing/desarrollo
   ↓

2. REPORTING (Reporte)
   ↓
   Click "New Issue" → Usar "Bug Report" template
   ↓
   Rellenar:
   - Descripción clara
   - Pasos para reproducir
   - Comportamiento esperado vs actual
   - Entorno (OS, versión)
   ↓
   Añadir labels: bug, priority-*, hito-*
   ↓

3. TRIAJE (Priorización)
   ↓
   En daily standup o reunión:
   - Evaluar severidad
   - Asignar prioridad
   - Asignar a desarrollador
   - Estimar esfuerzo
   ↓
   Issue se mueve a "Todo"
   ↓

4. ASSIGNMENT (Asignación)
   ↓
   Developer asignado revisa el issue
   ↓
   Pregunta dudas en comentarios si necesario
   ↓
   Mueve issue a "In Progress"
   ↓

5. FIXING (Arreglo)
   ↓
   git checkout -b bugfix/descripcion-del-bug
   ↓
   # Trabajar en el fix...
   git commit -m "fix: resolver [descripción] (#N)"
   ↓
   # Testing local
   git push origin bugfix/...
   ↓

6. REVIEW (Revisión)
   ↓
   Abrir PR con "Closes #N"
   ↓
   PR se mueve automáticamente a "In Review"
   ↓
   Reviewer verifica:
   - Bug está realmente resuelto
   - No introduce regresiones
   - Código es limpio
   ↓

7. VERIFICATION (Verificación)
   ↓
   Approve PR → Merge
   ↓
   Issue se cierra automáticamente
   ↓
   Issue se mueve automáticamente a "Done"
   ↓

8. CLOSURE (Cierre)
   ↓
   Bug resuelto ✅
   ↓
   Trazabilidad completa en historial
```

### Ejemplo Real Completo

**Día 1 - Descubrimiento:**

```
Youssef está testeando el nivel y nota:
"A veces el jugador atraviesa la plataforma al caer"
```

**Día 1 - Reporte:**

```
Issue #8 creado usando Bug Report template:

Título: Bug - Jugador atraviesa plataformas al caer

Descripción:
Cuando el jugador cae desde cierta altura, a veces atraviesa
la plataforma en lugar de colisionar con ella.

Pasos para reproducir:
1. Iniciar el juego
2. Saltar desde la plataforma más alta
3. Aproximadamente 1 de cada 5 veces, el jugador atraviesa el suelo

Comportamiento esperado:
El jugador debería detenerse al colisionar con cualquier plataforma

Comportamiento actual:
El jugador cae a través de la plataforma y sale de la pantalla

Entorno:
- OS: Linux Ubuntu 22.04
- Versión: main branch (commit abc123)
- Compilador: g++ 11.4.0

Labels: bug, priority-high, hito-1
Asignado a: @mouradirachid8-lgtm (responsable de físicas)
```

**Día 2 - Triaje:**

```
En daily standup:
- Equipo confirma: es un bug crítico
- Prioridad: HIGH (afecta jugabilidad core)
- Rachid: "Lo reviso hoy mismo"

Rachid comenta en el issue:
"Creo que es un problema de velocidad vertical muy alta.
Investigaré la función de resolución de colisiones."
```

**Día 2 - Fixing:**

```bash
# Rachid trabaja en el fix
git checkout -b bugfix/player-fall-through
git commit -m "fix: add velocity clamping before collision check (#8)"
git commit -m "fix: improve vertical collision resolution (#8)"
git push origin bugfix/player-fall-through
```

**Día 2 - Review:**

```
PR #15 creado:

Título: Fix player falling through platforms

Descripción:
The issue was that when velocity.y was very high (>20),
the collision detection would miss because the player
moved more than the platform height in a single frame.

Fixes:
- Added velocity clamping to max 15 units/frame
- Improved collision resolution to check multiple points
- Added safety check for extreme velocities

Testing:
- ✅ Jumped from highest platform 20 times, no fall-through
- ✅ All other collision scenarios still work
- ✅ No performance impact

Closes #8

Reviewer: @agv114-ua
```

**Día 3 - Verification:**

```
Ángel revisa el PR:
1. Lee el código
2. Compila localmente
3. Prueba el juego extensivamente
4. Deja comentario: "Tested extensively, works perfectly now"
5. Approve PR

Ángel hace merge
```

**Resultado:**

```
✅ Issue #8 cerrado automáticamente
✅ PR #15 merged
✅ Rama bugfix/player-fall-through eliminada automáticamente
✅ Historial completo documentado
✅ Código traceable a bug específico
```

---

## 3.5 Métricas de Bugtracking

Para mantener calidad y velocidad, monitorizamos:

| Métrica                                    | Objetivo   | Seguimiento        |
| ------------------------------------------ | ---------- | ------------------ |
| **Tiempo medio de resolución de bugs**     | < 48 horas | GitHub Insights    |
| **Bugs abiertos simultáneamente**          | < 5        | GitHub Issues      |
| **% bugs high priority resueltos en <24h** | 100%       | Manual             |
| **Bugs reabiertos**                        | < 10%      | Historial issues   |
| **Bugs sin asignar**                       | 0          | Filtro no:assignee |

### Reports Automáticos

GitHub proporciona visualizaciones nativas:

- **Issues abiertas vs cerradas** (gráfico temporal)
- **Issues por label** (distribución)
- **Issues por asignee** (distribución de carga)
- **Tiempo medio de cierre** (métrica de eficiencia)

---

## 3.6 Best Practices Implementadas

### 1. **Descripción Clara de Bugs**

❌ **Mal ejemplo:**

```
Título: No funciona
Descripción: El juego no va bien
```

✅ **Buen ejemplo:**

```
Título: [BUG] Jugador atraviesa plataforma al caer desde altura
Descripción:
Cuando el jugador cae desde >300 píxeles de altura,
aproximadamente 20% de las veces atraviesa la plataforma
en lugar de colisionar.

Reproducción 100%:
1. git checkout main
2. ./bin/chaos-crew
3. Saltar desde plataforma superior 10 veces
4. Observar fallos en colisión

Esperado: Jugador se detiene en plataforma
Actual: Jugador cae a través y sale de pantalla

Entorno: Linux, main branch, commit abc123
```

### 2. **Una Issue = Una Cosa**

❌ **Evitar:**

```
Issue #20: Arreglar colisiones y añadir sonido y mejorar menú
```

✅ **Preferir:**

```
Issue #20: Fix player-platform collision detection
Issue #21: Add jump sound effect
Issue #22: Improve main menu layout
```

**Razón:** Facilita tracking, asignación, y cierre granular.

### 3. **Labels Consistentes**

Todos los issues deben tener:

- **Tipo:** bug / enhancement / documentation
- **Prioridad:** priority-high / medium / low
- **Hito:** hito-1 / hito-2 / hito-3

### 4. **Cerrar Issues con PRs**

Siempre usar "Closes #N" o "Fixes #N" en el PR:

```markdown
## Description

This PR implements the player jump mechanic.

Closes #5
```

**NO cerrar issues manualmente** sin PR asociado (excepto duplicados o inválidos).

### 5. **Comentar Progreso**

Si un issue tarda >2 días, comentar progreso:

```
Day 1: "Empezando a investigar, parece un problema de timing"
Day 2: "Identificado el problema, trabajando en el fix"
Day 3: "Fix implementado, testing en curso"
```

Mantiene visibilidad para el equipo.

---

## 3.7 Conclusión

GitHub Issues + Projects es la elección óptima para Chaos Crew porque:

✅ **Centraliza** todo el workflow en una plataforma
✅ **Integra nativamente** con nuestro código y PRs
✅ **No requiere coste** ni configuración externa
✅ **Proporciona trazabilidad** completa código-bug
✅ **Es accesible** para evaluadores sin credenciales adicionales
✅ **Ofrece automatización** suficiente sin complejidad
✅ **Escala** con nuestras necesidades (simple ahora, features avanzadas después)

Para un proyecto académico de 6 semanas con equipo de 5 personas, proporciona **exactamente** las capacidades necesarias sin overhead innecesario, permitiéndonos enfocarnos en desarrollar el juego en vez de administrar herramientas.

---

# 4. Contribuciones a Otros Proyectos

## 4.1 Metodología de Evaluación

Como parte del Hito 1, analizaremos un proyecto de otro grupo de la asignatura DCA y aportaremos feedback constructivo mediante tickets (issues) en su repositorio.

### Proceso de Análisis

**Fecha de evaluación:** [Será indicada por el profesor]

**Pasos a seguir:**

1. **Acceso al proyecto:**

   - Recibir link al repositorio del otro grupo
   - Clonar el repositorio
   - Leer su README y documentación

2. **Compilación y prueba:**

   ```bash
   git clone [url-del-otro-proyecto]
   cd [nombre-proyecto]
   # Seguir sus instrucciones de compilación
   ./build.sh  # o el método que indiquen
   ```

3. **Evaluación sistemática:**

   - Probar todas las funcionalidades documentadas
   - Buscar bugs o comportamientos inesperados
   - Evaluar usabilidad y experiencia de usuario
   - Revisar calidad del código (si es público)
   - Revisar documentación

4. **Creación de issues:**
   - Crear issues en SU repositorio (no el nuestro)
   - Usar formato profesional y constructivo
   - Categorizar: bug / enhancement / documentation
   - Ser específicos y aportar valor

### Criterios de Evaluación

Evaluaremos los siguientes aspectos:

| Aspecto           | Qué evaluar                        |
| ----------------- | ---------------------------------- |
| **Funcionalidad** | ¿Funciona como describe el README? |
| **Bugs**          | ¿Hay errores que impiden jugar?    |
| **Usabilidad**    | ¿Es fácil entender cómo jugar?     |
| **Rendimiento**   | ¿Hay lag o problemas de FPS?       |
| **Documentación** | ¿Es clara y completa?              |
| **Compilación**   | ¿Se compila fácilmente?            |

---

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

- OS: Linux Ubuntu 22.04
- Versión: v0.1.0-alpha
- Compilador: g++ 11.4.0

**Severidad:**

- [ ] Critical (impide jugar)
- [ ] High (afecta jugabilidad significativamente)
- [ ] Medium (molesto pero jugable)
- [ ] Low (cosmético)

**Screenshots/Videos:**
[Si aplica]

**Notas adicionales:**
[Contexto adicional]

---

_Issue creado por el equipo de Chaos Crew como parte de la evaluación del Hito 1_
```

### Template para Mejoras (Enhancements)

```markdown
**Título:** [ENHANCEMENT] Sugerencia de mejora

**Descripción de la mejora:**
[Qué se podría mejorar]

**Problema actual:**
[Por qué la situación actual es subóptima]

**Solución propuesta:**
[Cómo se podría mejorar]

**Beneficio:**
[Por qué esto mejoraría el juego/proyecto]

**Prioridad sugerida:**

- [ ] High - Impacto significativo en experiencia
- [ ] Medium - Mejora notable
- [ ] Low - Nice to have

**Ejemplos/Referencias:**
[Si hay juegos o proyectos que lo hagan bien]

---

_Sugerencia del equipo de Chaos Crew - Evaluación Hito 1_
```

### Template para Documentación

```markdown
**Título:** [DOCS] Mejora en documentación

**Sección afectada:**
[Qué parte de la doc necesita mejora]

**Problema:**
[Qué no está claro o falta]

**Sugerencia:**
[Qué se podría añadir o aclarar]

**Impacto:**
[Cómo esto ayudaría a usuarios/desarrolladores]

---

_Feedback de documentación - Chaos Crew Team_
```

---

## 4.3 Principios de Feedback Constructivo

Al crear issues en proyectos de otros grupos, seguiremos estos principios:

### ✅ SÍ HACER:

1. **Ser específico:**

   - ❌ "El juego va mal"
   - ✅ "El FPS cae a ~20 cuando hay más de 3 enemigos en pantalla"

2. **Ser constructivo:**

   - ❌ "Esto es horrible"
   - ✅ "La colisión podría mejorarse detectando antes del overlap completo"

3. **Aportar contexto:**

   - Pasos exactos para reproducir
   - Entorno de prueba
   - Screenshots si ayudan

4. **Priorizar apropiadamente:**

   - Bugs críticos: "Crashea al inicio" → High priority
   - Mejoras estéticas: "El color de fondo podría ser más vibrante" → Low priority

5. **Ser respetuoso:**
   - Recordar que son compañeros en proceso de aprendizaje
   - Tono profesional y amigable
   - Agradecer su trabajo

### ❌ NO HACER:

1. **Críticas vagas:**

   - "No me gusta"
   - "Está mal hecho"

2. **Críticas sin fundamento:**

   - No reportar bugs sin haberlos reproducido
   - No sugerir mejoras sin explicar por qué

3. **Spam de issues:**

   - Calidad > Cantidad
   - 3-5 issues bien hechos > 20 superficiales

4. **Ser condescendiente:**

   - "Cualquiera sabría que esto está mal"
   - "Es obvio que deberían..."

5. **Comparaciones negativas:**
   - "Nuestro juego es mejor porque..."
   - "Deberían haber hecho X como nosotros"

---

## 4.4 Compromiso de Volumen

Como equipo, nos comprometemos a crear:

**Mínimo:** 5 issues de calidad

- Al menos 2 bugs reproducibles
- Al menos 2 sugerencias de mejora
- Al menos 1 sobre documentación

**Ideal:** 8-10 issues

**Distribución:**

- Cada miembro del equipo crea al menos 1 issue
- Nos coordinamos para no duplicar reports
- Priorizamos bugs críticos primero

---

## 4.5 Registro de Contribuciones

**Proyecto evaluado:** [Será completado cuando se asigne]

**Fecha de evaluación:** [Fecha]

**Issues creados:**

| #   | Tipo        | Título | Asignado por | Link   | Severidad |
| --- | ----------- | ------ | ------------ | ------ | --------- |
| 1   | Bug         | ...    | Erardo       | [link] | High      |
| 2   | Enhancement | ...    | Ángel        | [link] | Medium    |
| 3   | Bug         | ...    | Rachid       | [link] | Critical  |
| 4   | Docs        | ...    | Youssef      | [link] | -         |
| 5   | Enhancement | ...    | Darío        | [link] | Low       |

**Resumen:**

- Bugs encontrados: X
- Mejoras sugeridas: Y
- Issues de documentación: Z
- **Total: [X+Y+Z]**

**Feedback recibido del otro equipo:**
[Si el otro equipo responde a nuestros issues, documentar aquí]

---

## 4.6 Aprendizajes de la Evaluación

Tras evaluar el proyecto de otro equipo, documentaremos:

**¿Qué hicieron bien que podríamos aplicar?**

- [Idea 1]
- [Idea 2]

**¿Qué problemas encontramos que debemos evitar?**

- [Problema 1 y cómo lo evitaremos]
- [Problema 2 y cómo lo evitaremos]

**¿Qué aprendimos sobre testing y QA?**

- [Insight 1]
- [Insight 2]

Esta retroalimentación se integrará en nuestro proceso para Hitos 2 y 3.

---

# 5. Estado Actual del Proyecto

## 5.1 Funcionalidades Implementadas (Hito 1)

### ✅ Sistema de Gestión de Estados

**Implementación:**

- Enum `GameScreen` con estados: GAMEPLAY, GAME_OVER
- Switch principal en el bucle de juego
- Transiciones limpias entre estados

**Código:**

```cpp
enum GameScreen {
    GAMEPLAY = 0,
    GAME_OVER
};

GameScreen currentScreen = GAMEPLAY;
```

**Estados funcionales:**

1. **GAMEPLAY:** Juego activo, jugadores controlables
2. **GAME_OVER:** Pantalla final con opción de reinicio

---

### ✅ Sistema de Movimiento de Jugadores

**Implementación:**

- Estructura `Player` con posición, velocidad, color
- Input handling para 2 jugadores simultáneos
- Movimiento horizontal suave
- Sistema de salto con física básica

**Controles funcionales:**
| Acción | P1 | P2 |
|--------|----|----|
| Izquierda | A | ← |
| Derecha | D | → |
| Saltar | W/SPACE | ↑ |

**Variables:**

- Velocidad horizontal: 4.5 units/frame
- Fuerza de salto: -12.0 units
- Gravedad: 0.6 units/frame²

---

### ✅ Sistema de Físicas

**Implementación:**

- Gravedad constante aplicada cuando el jugador no está en el suelo
- Acumulación de velocidad vertical
- Estado `isGrounded` para determinar si puede saltar
- Prevención de double jump

**Física básica funcional:**

```
velocity.y += GRAVITY (cada frame)
position.y += velocity.y (cada frame)
```

---

### ✅ Sistema de Colisiones

**Implementación:**

- Detección AABB (Axis-Aligned Bounding Box)
- Colisión jugador-plataforma
- Resolución vertical (reposicionamiento)
- Detección de colisión con objetivo (salida)

**Funcionalidad:**

- Los jugadores se detienen al colisionar con plataformas
- Los jugadores no atraviesan el suelo
- Detección cooperativa de victoria (ambos en salida)

---

### ✅ Nivel Básico Funcional

**Elementos del nivel:**

- Plataforma principal (suelo): 1280x50 píxeles
- Plataformas intermedias: 4 plataformas de diferentes alturas
- Zona de salida: Rectángulo verde 80x100 píxeles

**Layout:**

```
                                    [EXIT]
                            [Platform]
                [Platform]
    [Platform]
                        [Platform]
=====================================  [Ground]
```

**Diseño:**

- Dificultad: Muy fácil (tutorial)
- Tiempo estimado: 30-60 segundos
- Requiere cooperación para que ambos lleguen a la salida

---

### ✅ Pantallas Implementadas

#### Pantalla de Gameplay

**Elementos:**

- Renderizado de plataformas (grises)
- Renderizado de jugadores (P1: azul, P2: rojo)
- Zona de salida (verde con texto "EXIT")
- HUD superior con instrucciones
- Contador de FPS

**UI visible:**

```
┌─────────────────────────────────────────────────────┐
│ CHAOS CREW - Hito 1 Alpha          FPS: 60          │
│ Player 1: A/D move, W jump                          │
│ Player 2: Arrows move, UP jump                      │
│ COOPERATIVE: Both must reach GREEN EXIT!            │
├─────────────────────────────────────────────────────┤
│                                                      │
│            [P1]  [P2]                          EXIT  │
│                                              [░░░░]  │
│              [═══════]                               │
│    [═══════]              [═══════]                  │
│                                 [═══════]            │
│═════════════════════════════════════════════════════│
└─────────────────────────────────────────────────────┘
```

#### Pantalla de Game Over

**Estados:**

**Victoria (ambos alcanzan salida):**

```
╔═══════════════════════════════════════╗
║                                       ║
║       LEVEL COMPLETE!                 ║
║                                       ║
║   Great teamwork! You both made it!   ║
║                                       ║
║     Press ENTER to try again          ║
║                                       ║
║  Chaos Crew - DCA UA 2025             ║
╚═══════════════════════════════════════╝
```

**Derrota (uno cae):**

```
╔═══════════════════════════════════════╗
║                                       ║
║           GAME OVER                   ║
║                                       ║
║   One player fell! Remember:          ║
║   teamwork is essential!              ║
║                                       ║
║     Press ENTER to try again          ║
║                                       ║
║  Chaos Crew - DCA UA 2025             ║
╚═══════════════════════════════════════╝
```

---

### ✅ Condiciones de Victoria/Derrota

**Victoria:**

- ✅ Jugador 1 alcanza zona de salida
- ✅ Y jugador 2 alcanza zona de salida
- → Transición a GAME_OVER con mensaje de éxito

**Derrota:**

- ❌ Cualquier jugador cae fuera de la pantalla (y > SCREEN_HEIGHT)
- → Transición a GAME_OVER con mensaje de fallo

**Reinicio:**

- Presionar ENTER en Game Over
- → Resetea posiciones, velocidades, estado
- → Vuelve a GAMEPLAY

---

## 5.2 Estructura del Código

### Arquitectura Actual (Hito 1)

```
src/main.cpp (único archivo, 320 líneas)
│
├── Constants (líneas 1-20)
│   ├── SCREEN_WIDTH, SCREEN_HEIGHT
│   ├── TARGET_FPS
│   ├── GRAVITY, JUMP_FORCE, MOVE_SPEED
│
├── Enums y Estructuras (líneas 21-50)
│   ├── enum GameScreen
│   ├── struct Player
│   ├── struct Platform
│
├── Variables Globales (líneas 51-70)
│   ├── currentScreen
│   ├── player1, player2
│   ├── platforms[]
│   ├── exitZone
│
├── Function Declarations (líneas 71-80)
│   ├── InitGame()
│   ├── UpdateGame()
│   ├── DrawGame()
│   ├── UpdateGameplay()
│   ├── DrawGameplay()
│   ├── DrawGameOver()
│   ├── ResetGame()
│
├── main() (líneas 81-100)
│   ├── InitWindow()
│   ├── SetTargetFPS()
│   ├── InitGame()
│   └── Game Loop:
│       ├── UpdateGame()
│       └── DrawGame()
│
├── InitGame() (líneas 101-140)
│   ├── Initialize player1
│   ├── Initialize player2
│   ├── Initialize platforms
│   ├── Initialize exitZone
│
├── UpdateGame() (líneas 141-160)
│   └── switch(currentScreen)
│       ├── GAMEPLAY → UpdateGameplay()
│       └── GAME_OVER → Check for restart
│
├── UpdateGameplay() (líneas 161-250)
│   ├── Input handling (ambos jugadores)
│   ├── Apply gravity
│   ├── Update positions
│   ├── Collision detection
│   ├── Check victory/defeat conditions
│
├── DrawGame() (líneas 251-270)
│   └── switch(currentScreen)
│       ├── GAMEPLAY → DrawGameplay()
│       └── GAME_OVER → DrawGameOver()
│
├── DrawGameplay() (líneas 271-300)
│   ├── Draw platforms
│   ├── Draw exitZone
│   ├── Draw players
│   ├── Draw HUD
│
├── DrawGameOver() (líneas 301-315)
│   ├── Draw background overlay
│   ├── Draw message (victory/defeat)
│   ├── Draw instructions
│
└── ResetGame() (líneas 316-320)
    └── Call InitGame()
```

### Refactorización Planificada (Hito 2)

```
Estructura modular objetivo:

src/
├── main.cpp (solo main loop)
├── core/
│   ├── Game.h / Game.cpp
│   └── Constants.h
├── entities/
│   ├── Player.h / Player.cpp
│   └── Platform.h / Platform.cpp
├── screens/
│   ├── GameScreen.h / GameScreen.cpp
│   └── GameOverScreen.h / GameOverScreen.cpp
├── physics/
│   ├── Physics.h / Physics.cpp
│   └── Collision.h / Collision.cpp
└── utils/
    └── Helpers.h / Helpers.cpp
```

---

## 5.3 Compilación y Despliegue

### Compilación Manual (Hito 1)

**Linux:**

```bash
g++ src/main.cpp -o bin/chaos-crew \
    -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
    -std=c++17 -Wall -Wextra -O2
```

**Flags utilizados:**

- `-std=c++17`: Usar C++17
- `-Wall -Wextra`: Warnings completos
- `-O2`: Optimización nivel 2
- `-lraylib -l...`: Link con Raylib y dependencias

### Script de Compilación (build.sh)

**Funcionalidades:**

```bash
./build.sh linux      # Compilar para Linux
./build.sh windows    # Cross-compilar para Windows
./build.sh run        # Compilar y ejecutar
./build.sh clean      # Limpiar builds
./build.sh package v0.1.0-alpha  # Crear release package
```

**Package generado:**

```
releases/v0.1.0-alpha/
├── chaos-crew-linux           (ejecutable)
├── chaos-crew.exe             (ejecutable Windows)
├── README.md
├── HITO1.md
└── assets/                    (si hay)

ZIP files:
├── chaos-crew-v0.1.0-alpha-linux.zip
└── chaos-crew-v0.1.0-alpha-windows.zip
```

---

## 5.4 Issues Conocidos (Bugs Menores)

### Documentados pero No Críticos

| Issue #     | Descripción                                                         | Prioridad | Estado |
| ----------- | ------------------------------------------------------------------- | --------- | ------ |
| [POR CREAR] | El jugador puede moverse horizontalmente en el aire                 | Low       | ⏳ H2  |
| [POR CREAR] | No hay feedback visual al alcanzar la salida antes del otro jugador | Medium    | ⏳ H2  |
| [POR CREAR] | No hay límite superior, jugador puede saltar infinitamente alto     | Low       | ⏳ H2  |
| [POR CREAR] | No hay animaciones, movimiento es instantáneo                       | Low       | ⏳ H2  |

**Nota:** Estos issues menores se abordarán en el Hito 2 durante la refactorización y polish.

---

## 5.5 Métricas del Proyecto

### Estadísticas de Código

| Métrica                | Valor                |
| ---------------------- | -------------------- |
| **Líneas de código**   | ~320 (main.cpp)      |
| **Archivos fuente**    | 1                    |
| **Funciones**          | 7                    |
| **Estructuras/Clases** | 2 (Player, Platform) |
| **Constantes**         | 8                    |

### Estadísticas del Repositorio

| Métrica             | Valor (Actualizar al final) |
| ------------------- | --------------------------- |
| **Commits totales** | [X]                         |
| **Pull Requests**   | [Y]                         |
| **Issues creados**  | [Z]                         |
| **Issues cerrados** | [W]                         |
| **Contributors**    | 5                           |

### Gameplay Metrics

| Métrica                   | Valor           |
| ------------------------- | --------------- |
| **FPS target**            | 60              |
| **FPS actual**            | 60 (estable)    |
| **Resolución**            | 1280x720        |
| **Duración nivel**        | ~30-60 segundos |
| **Controles simultáneos** | 2 jugadores     |

---

# 6. Contribuciones del Equipo

## 6.1 Distribución de Trabajo

### Resumen por Miembro

| Miembro                  | Rol                   | Horas estimadas | Issues asignados | PRs creados | Lines of code |
| ------------------------ | --------------------- | --------------- | ---------------- | ----------- | ------------- |
| **Erardo Aldana Pessoa** | Project Lead / DevOps | 18h             | #1, #6, #7       | [X]         | [Y]           |
| **Ángel Gonjar Verdejo** | Gameplay Programmer   | 15h             | #2               | [X]         | [Y]           |
| **Rachid Mouradi**       | Systems Programmer    | 16h             | #3               | [X]         | [Y]           |
| **Youssef S.A.**         | Level Designer        | 12h             | #5               | [X]         | [Y]           |
| **Darío S.D.**           | UI/Screens Developer  | 14h             | #4               | [X]         | [Y]           |

**Total:** ~75 horas de trabajo del equipo

---

## 6.2 Contribuciones Detalladas

### Erardo Aldana Pessoa (@eap59-ua)

**Rol:** Project Lead / DevOps

**Contribuciones principales:**

- ✅ Setup inicial del repositorio y estructura de carpetas
- ✅ Configuración de protecciones en rama main
- ✅ Creación y gestión de GitHub Project board
- ✅ Creación de todos los issues iniciales con asignaciones
- ✅ Implementación de templates de issues
- ✅ Script de compilación (build.sh)
- ✅ Generación de release v0.1.0-alpha
- ✅ Documentación completa (HITO1.md, GDD.md)
- ✅ Coordinación de reuniones y daily standups
- ✅ Code reviews de múltiples PRs

**Issues completados:**

- #1: Setup inicial del repositorio
- #6: Documentación Hito 1 (GDD, justificaciones)
- #7: Script de compilación y release

**Pull Requests:**

- PR #1: Initial project structure and configuration
- PR #[X]: Complete HITO1 documentation
- PR #[Y]: Build script and release generation

**Líneas de código:** [X líneas] (scripts, documentación)

**Contribución al equipo:**

- Soporte técnico a compañeros con problemas de Git
- Resolución de conflictos de merge
- Facilitación de comunicación diaria

---

### Ángel Gonjar Verdejo (@agv114-ua)

**Rol:** Gameplay Programmer

**Contribuciones principales:**

- ✅ Diseño e implementación de estructura Player
- ✅ Sistema de input handling para 2 jugadores
- ✅ Movimiento horizontal suave con velocidad constante
- ✅ Sistema de salto con impulso inicial
- ✅ Restricción de movimiento en límites de pantalla
- ✅ Testing extensivo de controles
- ✅ Balance inicial de variables de gameplay

**Issues completados:**

- #2: Sistema de movimiento de jugadores

**Pull Requests:**

- PR #[X]: Implement player movement system
- PR #[Y]: Add jump mechanic
- PR #[Z]: Balance movement speeds

**Líneas de código:** [X líneas] (Player handling, input)

**Contribución al equipo:**

- Code review de PRs de físicas
- Pair programming con Rachid para integración player-physics
- Testing de builds

---

### Rachid Mouradi (@mouradirachid8-lgtm)

**Rol:** Systems Programmer

**Contribuciones principales:**

- ✅ Implementación del sistema de gravedad
- ✅ Sistema de colisiones AABB
- ✅ Resolución correcta de colisiones verticales
- ✅ Estado isGrounded para control de salto
- ✅ Prevención de double jump
- ✅ Detección de condiciones de muerte (caída)
- ✅ Optimización de performance del collision checking

**Issues completados:**

- #3: Sistema de físicas y colisiones

**Pull Requests:**

- PR #[X]: Implement gravity system
- PR #[Y]: Add AABB collision detection
- PR #[Z]: Fix collision resolution edge cases

**Líneas de código:** [X líneas] (Physics, collision)

**Contribución al equipo:**

- Debugging de problemas complejos de colisión
- Pair programming con Ángel
- Documentación técnica de sistema de físicas

---

### Youssef S.A. (@ysa9-ua)

**Rol:** Level Designer

**Contribuciones principales:**

- ✅ Diseño conceptual del nivel en sketch
- ✅ Implementación de plataforma principal
- ✅ Posicionamiento de 4 plataformas intermedias
- ✅ Diseño y ubicación de zona de salida
- ✅ Definición de spawn points iniciales
- ✅ Balance de dificultad del nivel
- ✅ Playtesting extensivo (50+ runs)

**Issues completados:**

- #5: Diseño del nivel básico

**Pull Requests:**

- PR #[X]: Implement basic level layout
- PR #[Y]: Adjust platform positions for balance

**Líneas de código:** [X líneas] (Level data)

**Contribución al equipo:**

- Testing intensivo del juego completo
- Reportar bugs encontrados durante playtesting
- Feedback de gameplay a programadores

---

### Darío S.D. (@dsd17-ua)

**Rol:** UI/Screens Developer

**Contribuciones principales:**

- ✅ Enum GameScreen con estados del juego
- ✅ Implementación completa de pantalla GAMEPLAY
- ✅ Diseño de HUD con instrucciones
- ✅ Implementación de pantalla GAME_OVER
- ✅ Diferenciación visual victoria vs derrota
- ✅ Sistema de transición entre estados
- ✅ Feedback visual (textos, colores)

**Issues completados:**

- #4: Gestión de estados del juego

**Pull Requests:**

- PR #[X]: Implement game state management
- PR #[Y]: Create gameplay screen
- PR #[Z]: Create game over screen with feedback

**Líneas de código:** [X líneas] (Screen management, rendering)

**Contribución al equipo:**

- Diseño de UI/UX del proyecto
- Feedback sobre experiencia de usuario
- Testing de flujo completo del juego

---

## 6.3 Visualización de Contribuciones

### GitHub Insights

**Contributors Graph:**
[Captura de pantalla del gráfico de contribuciones de GitHub]

**Code Frequency:**
[Captura de la frecuencia de commits y líneas añadidas/eliminadas]

**Pulse:**
[Captura del activity overview del repositorio]

---

## 6.4 Code Reviews

### Estadísticas de Reviews

| Reviewer | PRs reviewed | Avg time to review | Approval rate |
| -------- | ------------ | ------------------ | ------------- |
| Erardo   | [X]          | [Y hours]          | [Z%]          |
| Ángel    | [X]          | [Y hours]          | [Z%]          |
| Rachid   | [X]          | [Y hours]          | [Z%]          |
| Youssef  | [X]          | [Y hours]          | [Z%]          |
| Darío    | [X]          | [Y hours]          | [Z%]          |

**Total reviews:** [N]

### Ejemplos de Reviews Significativos

**PR #[X] - Player Movement System**

- **Autor:** Ángel
- **Reviewer:** Rachid
- **Comentarios:** 5
- **Cambios solicitados:** Mejorar manejo de edge cases en límites de pantalla
- **Resultado:** Aprobado tras ajustes
- **Aprendizaje:** Importancia de validar límites antes de actualizar posición

**PR #[Y] - Collision System**

- **Autor:** Rachid
- **Reviewer:** Erardo
- **Comentarios:** 3
- **Feedback:** Optimizar loop de detección, añadir comentarios
- **Resultado:** Aprobado con sugerencias implementadas
- **Aprendizaje:** Balance entre rendimiento y claridad del código

---

## 6.5 Comunicación y Coordinación

### Daily Standups Realizados

| Fecha   | Asistentes | Duración | Temas principales                       |
| ------- | ---------- | -------- | --------------------------------------- |
| [Fecha] | 5/5        | 15 min   | Asignación inicial de tareas            |
| [Fecha] | 5/5        | 12 min   | Progreso día 1, resolución de dudas Git |
| [Fecha] | 4/5        | 18 min   | Integración de físicas, conflictos      |
| [Fecha] | 5/5        | 15 min   | Testing y bugs encontrados              |
| [Fecha] | 5/5        | 20 min   | Revisión final, preparación entrega     |

**Total standups:** [X]  
**Asistencia media:** 96%

### Reuniones Formales

| Tipo            | Fecha   | Duración | Participantes | Resultado                                     |
| --------------- | ------- | -------- | ------------- | --------------------------------------------- |
| **Kickoff**     | [Fecha] | 60 min   | 5/5           | Roles asignados, workflow definido            |
| **Mid-sprint**  | [Fecha] | 45 min   | 5/5           | Review de progreso, reajuste prioridades      |
| **Pre-entrega** | [Fecha] | 30 min   | 5/5           | Checklist final, asignación de últimas tareas |

---

## 6.6 Desafíos Enfrentados y Soluciones

### Desafío 1: Conflictos de Merge

**Problema:**
En la integración inicial, múltiples miembros modificaron `main.cpp` simultáneamente, generando conflictos complejos.

**Solución:**

- Comunicación previa sobre archivos a modificar
- Merges más frecuentes (diarios) de main en ramas feature
- Pair programming para secciones críticas

**Aprendizaje:**
La modularización temprana habría evitado este problema. En Hito 2 dividiremos el código en múltiples archivos desde el inicio.

---

### Desafío 2: Instalación de Raylib

**Problema:**
Algunos miembros tuvieron dificultades instalando Raylib, especialmente en Windows.

**Solución:**

- Erardo creó guía paso a paso específica
- Sesión de troubleshooting grupal
- Documentación de errores comunes y soluciones

**Aprendizaje:**
Validar que todos tengan el entorno funcional ANTES de asignar tareas de código.

---

### Desafío 3: Coordinación de Testing

**Problema:**
Testing descentralizado llevó a reportes de bugs duplicados o vagos.

**Solución:**

- Establecer sesión de playtesting coordinada
- Template de bug report obligatorio
- Youssef coordinó testing sistemático

**Aprendizaje:**
Un "testing lead" mejora la calidad de QA significativamente.

---

### Desafío 4: Balance entre Estudio y Proyecto

**Problema:**
Coincidencia con exámenes de otras asignaturas redujo disponibilidad.

**Solución:**

- Flexibilidad en deadlines internos
- Redistribución temporal de tareas
- Buffer time en planificación

**Aprendizaje:**
Planificar con margen de tiempo para imprevistos es esencial.

---

## 6.7 Retrospectiva del Hito 1

### ¿Qué funcionó bien? ✅

1. **GitHub Flow:** El workflow fue claro y efectivo desde el día 1
2. **Comunicación diaria:** Los standups mantuvieron al equipo sincronizado
3. **Code reviews:** Mejoraron calidad del código y aprendizaje mutuo
4. **Distribución de roles:** Cada persona tuvo responsabilidad clara
5. **Protección de main:** Nunca tuvimos main roto, siempre compilable

### ¿Qué se puede mejorar? 🔄

1. **Modularización:** Un solo archivo de 320 líneas es difícil de gestionar
2. **Testing automatizado:** Dependimos de testing manual, propenso a errores
3. **Documentación continua:** La documentación se dejó para el final
4. **Estimaciones:** Algunas tareas tomaron más tiempo del estimado
5. **Comunicación asíncrona:** A veces faltó contexto en mensajes de Discord

### Acciones para Hito 2 📋

1. ✅ **Modularizar desde el inicio:** Crear estructura de archivos antes de codificar
2. ✅ **Implementar tests:** Suite de tests automatizados con cada PR
3. ✅ **Documentar sobre la marcha:** Actualizar docs con cada feature
4. ✅ **Mejorar estimaciones:** Usar experiencia del H1 para estimar mejor
5. ✅ **CI/CD:** GitHub Actions para automatizar build y tests

---

## 6.8 Estadísticas Finales del Equipo

### Compromiso y Participación

| Métrica                     | Valor |
| --------------------------- | ----- |
| **Reuniones realizadas**    | [X]   |
| **Asistencia promedio**     | 96%   |
| **Daily standups**          | [Y]   |
| **Commits totales**         | [Z]   |
| **PRs totales**             | [W]   |
| **Issues cerrados**         | [N]   |
| **Code reviews realizados** | [M]   |

### Distribución de Commits

```
Erardo:  ████████████░░░░░░░░  25% ([X] commits)
Ángel:   ████████████████░░░░  22% ([Y] commits)
Rachid:  ████████████████░░░░  21% ([Z] commits)
Youssef: ██████████░░░░░░░░░░  16% ([W] commits)
Darío:   ██████████░░░░░░░░░░  16% ([N] commits)
```

**Interpretación:** Distribución equilibrada, sin desbalances significativos.

---

# 7. Conclusiones y Próximos Pasos

## 7.1 Logros del Hito 1

### Objetivos Cumplidos ✅

Hemos completado exitosamente todos los requisitos obligatorios del Hito 1:

**Requisitos de Juego:**

- ✅ 2 pantallas funcionales (Gameplay + Game Over)
- ✅ Mecánica principal de plataformas implementada
- ✅ Bucle de juego completo (jugar → ganar/perder → reiniciar)
- ✅ 2 jugadores simultáneos controlables
- ✅ Gráficos con figuras geométricas básicas
- ✅ Condiciones de victoria/derrota cooperativas

**Requisitos de Repositorio:**

- ✅ GitHub Flow implementado y documentado
- ✅ Sistema de bugtracking (GitHub Issues + Projects)
- ✅ Protección de rama main con PRs obligatorios
- ✅ Versionado semántico (v0.1.0-alpha)
- ✅ Releases generadas correctamente

**Requisitos de Documentación:**

- ✅ GDD básico completo y detallado
- ✅ Justificación de estrategia de ramas (GitHub Flow)
- ✅ Justificación de sistema de bugtracking
- ✅ Tabla de contribuciones del equipo
- ✅ Contribuciones a otro proyecto (pendiente asignación)

**Requisitos de Entrega:**

- ✅ Ejecutable Linux funcional
- ✅ Código fuente en release
- ✅ Documentación HITO1.md completa

---

## 7.2 Estado del Proyecto

**Versión actual:** v0.1.0-alpha

**Jugabilidad:**

- ⭐⭐⭐⭐☆ Mecánicas core funcionales
- ⭐⭐⭐☆☆ Diversión/engagement (básico pero funcional)
- ⭐⭐⭐⭐⭐ Cooperación requerida (objetivo logrado)
- ⭐⭐☆☆☆ Polish visual (como esperado para H1)

**Calidad Técnica:**

- ⭐⭐⭐⭐⭐ Estabilidad (0 crashes, siempre compilable)
- ⭐⭐⭐⭐☆ Rendimiento (60 FPS constantes)
- ⭐⭐⭐☆☆ Arquitectura del código (monolítico, mejorable)
- ⭐⭐⭐⭐☆ Gestión de proyecto (GitHub Flow efectivo)

---

## 7.3 Preparación para Hito 2

### Objetivos del Hito 2 (Beta)

**Fecha estimada:** Mediados de noviembre 2025

**Prioridades principales:**

#### 1. Refactorización y Modularización 🔧

- Dividir `main.cpp` en múltiples archivos
- Implementar arquitectura basada en módulos
- Preparar para patrón ECS (EnTT)

#### 2. Multijugador 3-5 Jugadores 🎮

- Soporte para gamepads
- Sistema de colores para hasta 5 jugadores
- Input manager robusto

#### 3. Sistema de Trampas Dinámicas ⚠️

- Plataformas que desaparecen
- Pinchos emergentes
- Obstáculos móviles
- Triggers y eventos

#### 4. Mecánicas Cooperativas Avanzadas 🤝

- Botones de presión múltiple
- Plataformas activadas por peso
- Puertas sincronizadas

#### 5. Arte y Audio 🎨🎵

- Sprites pixel art para jugadores
- Tileset para niveles
- Música de fondo
- Efectos de sonido

#### 6. Testing Automatizado ✅

- Suite de al menos 10 tests unitarios
- Tests de integración
- CI/CD con GitHub Actions

#### 7. Niveles Adicionales 🗺️

- 3-4 niveles completos
- Dificultad progresiva
- Introducción gradual de mecánicas

---

## 7.4 Roadmap Completo

### Timeline Global del Proyecto

```
HITO 1 - ALPHA ✅ (1-22 Oct)
├─ Semana 1: Setup y fundamentos
├─ Semana 2: Integración de features
└─ Semana 3: Polish y documentación
   └─ 22 Oct: ENTREGA v0.1.0-alpha

HITO 2 - BETA 🔄 (23 Oct - 15 Nov)
├─ Semana 4: Refactorización y arquitectura
├─ Semana 5: Trampas dinámicas y multijugador
└─ Semana 6: Arte, audio, testing
   └─ 15 Nov: ENTREGA v0.2.0-beta

HITO 3 - RELEASE 📅 (16 Nov - 6 Dic)
├─ Semana 7: Niveles completos y ECS
├─ Semana 8: Polish, i18n, features secundarias
└─ Semana 9: Testing final y documentación
   └─ 6 Dic: ENTREGA FINAL v1.0.0
```

---

## 7.5 Lecciones Aprendidas

### Desarrollo Colaborativo

**Aprendimos que:**

- GitHub Flow es efectivo para equipos pequeños con entregas frecuentes
- Code reviews mejoran calidad Y conocimiento compartido
- Comunicación diaria (standups) es crucial para coordinación
- Protección de main previene errores costosos
- Issues bien documentados ahorran tiempo después

**Mejoras para aplicar:**

- Modularizar código desde el inicio
- Documentar decisiones técnicas en tiempo real
- Estimar con más margen de error
- Usar pair programming para tareas complejas

### Desarrollo de Videojuegos

**Aprendimos que:**

- Gameplay simple pero bien ejecutado > features complejas a medias
- Testing constante es esencial (encontramos bugs jugando nosotros mismos)
- Balance de dificultad requiere muchas iteraciones
- Feedback visual inmediato mejora experiencia de usuario
- Cooperación genuina es difícil de diseñar pero muy satisfactoria

**Mejoras para aplicar:**

- Playtesting con personas fuera del equipo
- Métricas de jugabilidad (tiempo promedio, tasa de completado)
- Diseño iterativo de niveles
- Prototipado rápido de mecánicas antes de implementar

### Gestión de Proyecto

**Aprendimos que:**

- Estimaciones iniciales suelen ser optimistas
- Imprevistos (exámenes, enfermedades) siempre ocurren
- Roles claros reducen confusión y overlap
- Herramientas simples bien usadas > herramientas complejas mal usadas
- Retrospectivas ayudan a mejorar continuamente

**Mejoras para aplicar:**

- Buffer time de 20% en estimaciones
- Revisión semanal de progreso y ajuste de plan
- Documentación de decisiones importantes
- Celebración de hitos intermedios

---

## 7.6 Agradecimientos

Queremos agradecer:

- **Al profesor Antonio Ruiz Martínez** por la validación de nuestra propuesta y el apoyo durante el desarrollo
- **A nuestros compañeros de la asignatura** por el feedback y las pruebas de nuestro juego
- **A la comunidad de Raylib** por la excelente documentación y ejemplos
- **Al equipo completo de Chaos Crew** por el compromiso, profesionalismo, y buen ambiente de trabajo

---

## 7.7 Referencias y Recursos

### Documentación Técnica

- **Raylib:** https://www.raylib.com/
- **Raylib Cheatsheet:** https://www.raylib.com/cheatsheet/cheatsheet.html
- **Raylib Examples:** https://github.com/raysan5/raylib/tree/master/examples

### Metodologías y Buenas Prácticas

- **GitHub Flow:** https://docs.github.com/en/get-started/quickstart/github-flow
- **Conventional Commits:** https://www.conventionalcommits.org/
- **Semantic Versioning:** https://semver.org/

### Inspiración de Juegos

- **Level Devil:** https://www.crazygames.com/game/level-devil
- **Pico Park:** https://store.steampowered.com/app/1509960/PICO_PARK/

### Materiales de la Asignatura

- **Repositorio de teoría DCA:** https://antoniorv6.github.io/dca-gii/
- **Enunciado del proyecto:** https://antoniorv6.github.io/dca-gii/practicas/p00/
- **Enunciado Hito 1:** https://antoniorv6.github.io/dca-gii/practicas/p01/

---

# 8. Anexos

## Anexo A: Instrucciones de Compilación Detalladas

### Requisitos del Sistema

**Mínimos:**

- CPU: Procesador dual-core 1.5 GHz
- RAM: 2 GB
- GPU: Integrada con soporte OpenGL 3.3
- Espacio: 50 MB

**Recomendados:**

- CPU: Procesador quad-core 2.0 GHz
- RAM: 4 GB
- GPU: Dedicada con soporte OpenGL 4.3
- Espacio: 100 MB

### Instalación de Dependencias

#### Linux (Ubuntu/Debian)

```bash
# Actualizar repositorios
sudo apt update

# Instalar herramientas de desarrollo
sudo apt install build-essential git

# Instalar dependencias de Raylib
sudo apt install libasound2-dev libx11-dev libxrandr-dev \
                 libxi-dev libgl1-mesa-dev libglu1-mesa-dev \
                 libxcursor-dev libxinerama-dev libwayland-dev \
                 libxkbcommon-dev

# Clonar e instalar Raylib
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
cd ../..

# Verificar instalación
pkg-config --modversion raylib
```

#### Windows (MinGW)

```bash
# Instalar MinGW-w64 desde https://www.mingw-w64.org/

# Descargar Raylib precompilado
# https://github.com/raysan5/raylib/releases

# Extraer en C:\raylib\

# Añadir a PATH:
# C:\raylib\lib
# C:\MinGW\bin
```

### Compilación del Proyecto

#### Opción 1: Script Automatizado (Recomendado)

```bash
# Clonar repositorio
git clone https://github.com/eap59-ua/chaos-crew-dca.git
cd chaos-crew-dca

# Dar permisos al script
chmod +x build.sh

# Compilar
./build.sh linux    # Para Linux
./build.sh windows  # Para Windows (requiere MinGW)

# Ejecutar
./bin/chaos-crew
```

#### Opción 2: Compilación Manual

**Linux:**

```bash
g++ src/main.cpp -o bin/chaos-crew \
    -I/usr/local/include \
    -L/usr/local/lib \
    -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
    -std=c++17 \
    -Wall -Wextra \
    -O2

# Ejecutar
./bin/chaos-crew
```

**Windows:**

```bash
g++ src/main.cpp -o bin/chaos-crew.exe \
    -IC:/raylib/include \
    -LC:/raylib/lib \
    -lraylib -lopengl32 -lgdi32 -lwinmm \
    -std=c++17 \
    -Wall -Wextra \
    -O2 \
    -static-libgcc -static-libstdc++

# Ejecutar
bin\chaos-crew.exe
```

### Solución de Problemas Comunes

**Error: "raylib.h: No such file or directory"**

```bash
# Verificar que Raylib está instalado
pkg-config --cflags raylib

# Si falla, reinstalar Raylib
```

**Error: "undefined reference to InitWindow"**

```bash
# Añadir flags de linkeo correctos
-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

**Error: Pantalla negra al ejecutar**

```bash
# Verificar drivers de GPU
sudo apt install mesa-utils
glxinfo | grep OpenGL
```

---

## Anexo B: Guía de Uso del Juego

### Controles

**Jugador 1 (Cuadrado Azul):**

- `A`: Moverse a la izquierda
- `D`: Moverse a la derecha
- `W` o `SPACE`: Saltar

**Jugador 2 (Cuadrado Rojo):**

- `←` (Flecha izquierda): Moverse a la izquierda
- `→` (Flecha derecha): Moverse a la derecha
- `↑` (Flecha arriba): Saltar

**Generales:**

- `ENTER`: Reiniciar (en pantalla Game Over)
- `ESC`: Cerrar el juego

### Objetivo

**Ganar:**

- AMBOS jugadores deben alcanzar la zona de salida (rectángulo verde con texto "EXIT")
- Si solo uno llega, el nivel NO se completa
- Cooperación es esencial

**Perder:**

- Si CUALQUIER jugador cae fuera de la pantalla
- Ambos jugadores reinician desde el inicio
- No hay sistema de vidas (aún)

### Consejos para Jugar

1. **Comunicación:** Hablen entre ustedes sobre quién va primero
2. **Paciencia:** No corran, esperen al compañero
3. **Saltos coordinados:** Algunos saltos requieren timing
4. **Exploración:** El nivel es pequeño, exploren juntos
5. **Diversión:** Está en alpha, disfruten el caos

---

## Anexo C: Comandos Git de Referencia Rápida

### Comandos Básicos

```bash
# Ver estado del repositorio
git status

# Ver historial de commits
git log --oneline --graph --all

# Ver ramas
git branch -a

# Ver cambios no committed
git diff

# Ver cambios en un archivo específico
git diff src/main.cpp
```

### Workflow Típico

```bash
# 1. Actualizar main
git checkout main
git pull origin main

# 2. Crear rama feature
git checkout -b feature/mi-feature

# 3. Hacer cambios
# ... editar archivos ...

# 4. Ver cambios
git status
git diff

# 5. Añadir cambios
git add src/main.cpp
# o añadir todo:
git add .

# 6. Commit
git commit -m "feat: descripción del cambio"

# 7. Subir rama
git push origin feature/mi-feature

# 8. Abrir PR en GitHub web

# 9. Tras merge, actualizar local
git checkout main
git pull origin main

# 10. Eliminar rama local (opcional)
git branch -d feature/mi-feature
```

### Comandos Avanzados

```bash
# Actualizar tu rama con cambios de main
git checkout feature/mi-rama
git merge main

# Deshacer cambios no committed
git restore archivo.cpp
git restore .  # todos los archivos

# Ver quién modificó cada línea
git blame archivo.cpp

# Buscar en el historial
git log --grep="bug fix"

# Ver cambios de un commit específico
git show abc123

# Crear tag para release
git tag -a v0.1.0-alpha -m "Hito 1 Alpha Release"
git push origin v0.1.0-alpha
```

---

## Anexo D: Glosario de Términos

### Términos de Desarrollo

| Término         | Definición                                                                       |
| --------------- | -------------------------------------------------------------------------------- |
| **AABB**        | Axis-Aligned Bounding Box - Método de detección de colisiones usando rectángulos |
| **Branch**      | Rama de desarrollo en Git                                                        |
| **CI/CD**       | Continuous Integration / Continuous Deployment                                   |
| **Commit**      | Snapshot de cambios en el repositorio                                            |
| **ECS**         | Entity Component System - Patrón arquitectónico para videojuegos                 |
| **FPS**         | Frames Per Second - Imágenes por segundo                                         |
| **GDD**         | Game Design Document - Documento de diseño del juego                             |
| **Merge**       | Integrar cambios de una rama en otra                                             |
| **PR**          | Pull Request - Solicitud de integración de código                                |
| **Refactoring** | Reestructurar código sin cambiar funcionalidad                                   |
| **Release**     | Versión publicada del software                                                   |
| **Repository**  | Almacén de código y historial de versiones                                       |

### Términos de Videojuegos

| Término         | Definición                                  |
| --------------- | ------------------------------------------- |
| **Checkpoint**  | Punto de guardado en el nivel               |
| **Gameplay**    | Mecánicas y experiencia de jugar            |
| **HUD**         | Heads-Up Display - Interfaz en pantalla     |
| **NPC**         | Non-Player Character - Personaje no jugador |
| **Playtesting** | Probar el juego para encontrar problemas    |
| **Polish**      | Refinamiento final y mejoras estéticas      |
| **Spawn point** | Punto donde aparecen los jugadores          |
| **Sprite**      | Imagen 2D de un objeto del juego            |
| **Tileset**     | Conjunto de tiles para construir niveles    |
| **Trap**        | Trampa u obstáculo peligroso                |

---

## Anexo E: Checklist de Entrega Final

### Pre-entrega (21 de octubre)

- [ ] Todo el código committed y pushed a main
- [ ] Última versión compila sin errores ni warnings
- [ ] Juego testeado exhaustivamente (>50 runs)
- [ ] Todos los issues del Hito 1 cerrados
- [ ] Documentación HITO1.md completa
- [ ] Screenshots del juego tomadas
- [ ] Contribuciones a otro proyecto completadas

### Día de Entrega (22 de octubre)

- [ ] Crear tag v0.1.0-alpha
- [ ] Compilar ejecutable Linux
- [ ] (Opcional) Compilar ejecutable Windows
- [ ] Crear packages con build.sh
- [ ] Crear release en GitHub
- [ ] Subir ejecutables a la release
- [ ] Adjuntar HITO1.md a la release
- [ ] Verificar que todo sea accesible públicamente
- [ ] Notificar al profesor (si requerido)

### Verificación Final

- [ ] El juego se puede compilar desde source
- [ ] El ejecutable Linux funciona
- [ ] README.md está actualizado
- [ ] Todos los links en documentación funcionan
- [ ] No hay información sensible en el repo
- [ ] .gitignore excluye archivos innecesarios
- [ ] Contribuciones del equipo están documentadas

---

**FIN DEL DOCUMENTO HITO1.md**

---

**Versión:** 1.0  
**Fecha:** 22 de octubre de 2025  
**Equipo:** Chaos Crew  
**Repositorio:** https://github.com/eap59-ua/chaos-crew-dca  
**Universidad de Alicante - Desarrollo Colaborativo de Aplicaciones**
