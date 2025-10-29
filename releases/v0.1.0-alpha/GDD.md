# Game Design Document (GDD)

## Chaos Crew - Cooperative Platformer

**Versión:** 1.0  
**Fecha:** 9 de octubre de 2025  
**Equipo:** Chaos Crew - Universidad de Alicante  
**Asignatura:** Desarrollo Colaborativo de Aplicaciones

---

## 1. Información General

### 1.1 Concepto del Juego

| Atributo        | Valor                      |
| --------------- | -------------------------- |
| **Título**      | Chaos Crew                 |
| **Género**      | Plataformas 2D cooperativo |
| **Plataforma**  | PC (Linux/Windows)         |
| **Jugadores**   | 2-5 (multijugador local)   |
| **Tecnología**  | C++17 con Raylib           |
| **Perspectiva** | Vista lateral 2D           |

### 1.2 Concepto Core

**Chaos Crew** es un juego de plataformas cooperativo donde 2 a 5 jugadores deben trabajar en equipo para superar niveles llenos de obstáculos y trampas que pueden cambiar dinámicamente durante la partida.

**Inspiración:**

- **Level Devil:** Mecánicas de trampas dinámicas e impredecibles
- **Pico Park:** Cooperación obligatoria entre jugadores

**Característica distintiva:** **Ambos jugadores deben llegar a la salida** para completar el nivel. Si uno falla, todos reinician, fomentando el trabajo en equipo genuino.

### 1.3 Propuesta de Valor

- ✅ **Cooperación genuina:** No opcional, sino necesaria
- ✅ **Trampas impredecibles:** El nivel cambia constantemente
- ✅ **Accesibilidad:** Controles simples (mover + saltar)
- ✅ **Partidas rápidas:** Niveles de 2-3 minutos
- ✅ **Escalabilidad:** Funciona con 2-5 jugadores

---

## 2. Mecánicas del Juego

### 2.1 Mecánicas Core (Hito 1 - Alpha)

#### Movimiento

- **Horizontal:** Izquierda/Derecha con velocidad constante (4.5 units/frame)
- **Salto:** Impulso inicial (-12.0 units) con gravedad aplicada (0.6 units/frame²)
- **Restricciones:** Los jugadores no salen de los límites de pantalla

#### Física

- **Gravedad:** Aplicada constantemente cuando no están en suelo
- **Estado "en suelo":** Determina si pueden saltar (previene double jump)
- **Velocidad vertical:** Acumulada por gravedad y salto

#### Colisiones

- **Detección AABB:** Rectángulos de colisión
- **Resolución vertical:** Reposicionamiento encima de plataformas
- **Colisión con salida:** Detecta victoria cooperativa

#### Condiciones de Victoria/Derrota

- **Victoria:** ✅ Ambos jugadores en zona de salida
- **Derrota:** ❌ Cualquier jugador cae fuera de pantalla

### 2.2 Mecánicas Planificadas (Hito 2+)

#### Trampas Dinámicas

- [ ] Plataformas que desaparecen temporalmente
- [ ] Pinchos emergentes con timers
- [ ] Plataformas móviles
- [ ] Obstáculos activados por triggers

#### Mecánicas Cooperativas Avanzadas

- [ ] Botones de presión múltiple (requieren 2+ jugadores)
- [ ] Plataformas activadas por peso
- [ ] Puertas sincronizadas
- [ ] Palancas temporales (uno mantiene, otro avanza)
- [ ] Boost cooperativo (impulsar a otro jugador)

#### Sistema de Checkpoints

- [ ] Puntos de guardado compartidos
- [ ] Respawn en checkpoint
- [ ] Indicador visual de progreso

---

## 3. Niveles y Progresión

### 3.1 Nivel 1 - Tutorial (Hito 1)

**Objetivo:** Enseñar mecánicas básicas

**Elementos:**

- Plataforma principal (suelo)
- 3-5 plataformas intermedias
- Zona de salida claramente marcada

**Dificultad:** Muy fácil, sin trampas  
**Duración:** 30-60 segundos

### 3.2 Niveles Futuros (Hito 2-3)

**Niveles 2-4:** Introducción progresiva de trampas y mecánicas cooperativas simples

**Niveles 5-8:** Combinación de todas las mecánicas, desafíos cooperativos complejos

---

## 4. Controles

### 4.1 Hito 1 (2 Jugadores - Teclado)

| Acción          | Jugador 1     | Jugador 2 |
| --------------- | ------------- | --------- |
| Mover izquierda | `A`           | `←`       |
| Mover derecha   | `D`           | `→`       |
| Saltar          | `W` o `SPACE` | `↑`       |
| Reiniciar       | `ENTER`       | `ENTER`   |

### 4.2 Hito 2+ (3-5 Jugadores - Gamepads)

- Jugador 3-5: Soporte para gamepads
- Stick izquierdo / D-Pad: Movimiento
- Botón A / Cross: Saltar
- Start: Pausa

---

## 5. Arte y Estilo Visual

### 5.1 Estilo Visual (Hito 1 - Figuras Geométricas)

| Elemento    | Representación    | Color       |
| ----------- | ----------------- | ----------- |
| Jugador 1   | Cuadrado 30x40    | Azul        |
| Jugador 2   | Cuadrado 30x40    | Rojo        |
| Plataformas | Rectángulos       | Gris        |
| Suelo       | Rectángulo grande | Gris oscuro |
| Salida      | Rectángulo 80x100 | Verde       |
| Fondo       | Sólido            | Blanco      |

**Indicadores:**

- Texto "P1" y "P2" dentro de jugadores
- Texto "EXIT" en zona de salida
- Instrucciones en HUD superior

### 5.2 Estilo Visual (Hito 2+ - Pixel Art)

**Objetivo:** Pixel art colorido inspirado en juegos retro

- Sprites animados para jugadores (idle, run, jump)
- Tileset para plataformas
- Animaciones para trampas
- Efectos de partículas
- Paleta vibrante y alegre

**Referencias de estilo:**

- Celeste (claridad visual)
- Super Meat Boy (precisión)
- Juegos Pico-8 (paleta limitada)

---

## 6. Audio (Planificado - Hito 2+)

### 6.1 Música

- [ ] Tema principal (menú)
- [ ] Música de gameplay (energética, loop)
- [ ] Música de victoria
- [ ] Música de derrota

### 6.2 Efectos de Sonido

- [ ] Salto (`jump.wav`)
- [ ] Aterrizaje (`land.wav`)
- [ ] Colisión con trampa (`hurt.wav`)
- [ ] Victoria (`win.wav`)
- [ ] Botón activado (`button.wav`)
- [ ] Checkpoint (`checkpoint.wav`)
- [ ] UI navigation

---

## 7. Tecnologías

### 7.1 Core

- **Lenguaje:** C++17
- **Framework:** Raylib 5.0
- **Build:** Script bash (Hito 1), CMake (Hito 2+)
- **Control de versiones:** Git + GitHub

### 7.2 Librerías Adicionales (Planificadas)

| Librería   | Uso                          | Hito   |
| ---------- | ---------------------------- | ------ |
| **EnTT**   | Patrón ECS                   | Hito 2 |
| **spdlog** | Sistema de logging           | Hito 2 |
| **Box2D**  | Físicas avanzadas (opcional) | Hito 3 |
| **ImGui**  | Interfaz de debug            | Hito 3 |

---

## 8. Pilares de Diseño

### 8.1 Principios Fundamentales

1. **Cooperación Genuina**
   - No opcional, es necesaria
   - Fallo de uno = fallo de todos
2. **Accesibilidad**
   - Controles simples
   - Fácil de aprender, difícil de dominar
3. **Diversión Compartida**
   - Momentos de caos controlado
   - Celebración de logros en equipo
4. **Iteración Rápida**
   - Niveles cortos (2-3 min)
   - Reinicio rápido tras fallo
5. **Sorpresa y Variedad**
   - Trampas impredecibles
   - Cada nivel presenta nuevos desafíos

### 8.2 Público Objetivo

- **Edad:** 8-35 años
- **Perfil:** Jugadores casuales que buscan diversión cooperativa local
- **Experiencia:** No requiere experiencia previa
- **Contexto:** Sesiones con amigos/familia

---

## 9. Alcance del Proyecto

### 9.1 Hito 1 - Alpha (22 Oct 2025)

✅ Prototipo jugable mínimo

- 2 jugadores
- Mecánicas básicas de plataformas
- 1 nivel funcional
- 2 pantallas (Gameplay + Game Over)

### 9.2 Hito 2 - Beta (Mediados Nov 2025)

🔄 Mecánicas completas y pulidas

- 3-5 jugadores
- Sistema de trampas dinámicas
- 3-4 niveles
- Sprites y audio
- Tests automatizados

### 9.3 Hito 3 - Release (Principios Dic 2025)

📅 Producto completo

- 5-8 niveles completos
- Patrón ECS
- Internacionalización (ES/EN)
- Ejecutables Linux y Windows
- Sistema de logging
- Polish completo

---

## 10. Métricas de Éxito

### 10.1 Métricas de Jugabilidad

- **Tasa de completado:** >70% de los jugadores completan nivel 1
- **Tiempo promedio:** 30-60 segundos por nivel
- **Diversión:** Feedback cualitativo positivo en playtesting

### 10.2 Métricas Técnicas

- **Rendimiento:** 60 FPS constantes
- **Estabilidad:** 0 crashes críticos
- **Cobertura de tests:** >80% del código core

---

## 11. Riesgos y Mitigación

| Riesgo                            | Probabilidad | Impacto | Mitigación                  |
| --------------------------------- | ------------ | ------- | --------------------------- |
| Físicas complejas de implementar  | Media        | Alto    | Usar Box2D si necesario     |
| Dificultad de balanceo de niveles | Alta         | Medio   | Playtesting frecuente       |
| Problemas de rendimiento          | Baja         | Alto    | Profiling temprano          |
| Conflictos de merge en código     | Media        | Medio   | Modularización desde inicio |

---

## 12. Referencias

### 12.1 Juegos de Inspiración

- **Level Devil:** https://www.crazygames.com/game/level-devil
- **Pico Park:** https://store.steampowered.com/app/1509960/PICO_PARK/
- **Celeste:** Mecánicas de plataformas precisas
- **Super Meat Boy:** Feedback inmediato

### 12.2 Recursos Técnicos

- **Raylib:** https://www.raylib.com/
- **Raylib Examples:** https://github.com/raysan5/raylib/tree/master/examples
- **EnTT:** https://github.com/skypjack/entt

---

## 13. Glosario

| Término         | Definición                                      |
| --------------- | ----------------------------------------------- |
| **AABB**        | Axis-Aligned Bounding Box - Método de colisión  |
| **Checkpoint**  | Punto de guardado en el nivel                   |
| **ECS**         | Entity Component System - Patrón arquitectónico |
| **FPS**         | Frames Per Second - Imágenes por segundo        |
| **Gameplay**    | Mecánicas y experiencia de jugar                |
| **HUD**         | Heads-Up Display - Interfaz en pantalla         |
| **Playtesting** | Probar el juego para encontrar problemas        |
| **Sprite**      | Imagen 2D de un objeto del juego                |

---

**Documento vivo:** Este GDD se actualizará conforme el proyecto evolucione.

**Última actualización:** 9 de octubre de 2025  
**Equipo:** Chaos Crew - DCA Universidad de Alicante
