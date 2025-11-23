#pragma once
#include <raylib.h>

struct Sprite {
    Texture2D texture;  // La textura actual que se está dibujando
    Vector2 offset;     // Ajuste visual (X, Y) para centrar el sprite en el hitbox
    
    // Dimensiones y transformación
    float frameWidth;   // Ancho de un solo frame (ej: 32.0f)
    float frameHeight;  // Alto de un solo frame (ej: 32.0f)
    float scale;        // Escala (ej: 2.0f para verlo al doble de tamaño)
    
    bool flipX;         // True = Mirar a la izquierda (invertir textura)

    // --- NUEVO: CONTROL DE ANIMACIÓN ---
    // Estos campos son necesarios para recorrer las tiras de imágenes (Sprite Sheets)
    int totalFrames;    // Cuántos cuadros tiene la animación actual (ej: Run=12, Idle=11)
    int currentFrame;   // En qué cuadro vamos (0, 1, 2...)
    float frameTime;    // Velocidad: Cuántos segundos dura cada cuadro (ej: 0.05f)
    float timer;        // Acumulador de tiempo interno
};