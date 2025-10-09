/*******************************************************************************************
*
*   Chaos Crew - Cooperative Platformer Game
*   
*   Hito 1 - Alpha Version
*   Universidad de Alicante - Desarrollo Colaborativo de Aplicaciones
*   
*   Equipo: Erardo Aldana, Ángel Gonjar, Rachid Mouradi, Youssef S.A., Darío S.D.
*
********************************************************************************************/

#include "raylib.h"

//------------------------------------------------------------------------------------
// Constants and Configuration
//------------------------------------------------------------------------------------
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

constexpr float GRAVITY = 0.6f;
constexpr float JUMP_FORCE = -12.0f;
constexpr float MOVE_SPEED = 4.5f;

//------------------------------------------------------------------------------------
// Game States
//------------------------------------------------------------------------------------
enum GameScreen {
    GAMEPLAY = 0,
    GAME_OVER
};

//------------------------------------------------------------------------------------
// Player Structure
//------------------------------------------------------------------------------------
struct Player {
    Rectangle rect;
    Vector2 velocity;
    Color color;
    bool isGrounded;
    int id;
    
    // Input keys
    int leftKey;
    int rightKey;
    int jumpKey;
};

//------------------------------------------------------------------------------------
// Game Objects
//------------------------------------------------------------------------------------
struct Platform {
    Rectangle rect;
    Color color;
};

//------------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------------
void InitGame();
void UpdateGame();
void DrawGame();
void UpdateGameplay();
void DrawGameplay();
void DrawGameOver();
void ResetGame();

//------------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------------
GameScreen currentScreen = GAMEPLAY;

Player player1;
Player player2;

Platform platforms[10];
int platformCount = 0;

Rectangle exitZone;
bool levelCompleted = false;

//------------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chaos Crew - Hito 1 Alpha");
    SetTargetFPS(TARGET_FPS);
    
    InitGame();
    
    // Main game loop
    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }
    
    // Cleanup
    CloseWindow();
    
    return 0;
}

//------------------------------------------------------------------------------------
// Initialize Game
//------------------------------------------------------------------------------------
void InitGame() {
    // Initialize Player 1 (Blue)
    player1.rect = { 100, SCREEN_HEIGHT - 200, 30, 40 };
    player1.velocity = { 0, 0 };
    player1.color = BLUE;
    player1.isGrounded = false;
    player1.id = 1;
    player1.leftKey = KEY_A;
    player1.rightKey = KEY_D;
    player1.jumpKey = KEY_W;
    
    // Initialize Player 2 (Red)
    player2.rect = { 150, SCREEN_HEIGHT - 200, 30, 40 };
    player2.velocity = { 0, 0 };
    player2.color = RED;
    player2.isGrounded = false;
    player2.id = 2;
    player2.leftKey = KEY_LEFT;
    player2.rightKey = KEY_RIGHT;
    player2.jumpKey = KEY_UP;
    
    // Initialize Platforms
    platformCount = 0;
    
    // Ground platform
    platforms[platformCount++] = {{ 0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 50 }, DARKGRAY};
    
    // Middle platforms
    platforms[platformCount++] = {{ 200, SCREEN_HEIGHT - 200, 200, 20 }, GRAY};
    platforms[platformCount++] = {{ 500, SCREEN_HEIGHT - 300, 200, 20 }, GRAY};
    platforms[platformCount++] = {{ 800, SCREEN_HEIGHT - 400, 200, 20 }, GRAY};
    platforms[platformCount++] = {{ 1000, SCREEN_HEIGHT - 250, 150, 20 }, GRAY};
    
    // Exit zone (green rectangle at the end)
    exitZone = { SCREEN_WIDTH - 100, SCREEN_HEIGHT - 150, 80, 100 };
    
    levelCompleted = false;
    currentScreen = GAMEPLAY;
}

//------------------------------------------------------------------------------------
// Update Game (Main update function)
//------------------------------------------------------------------------------------
void UpdateGame() {
    switch(currentScreen) {
        case GAMEPLAY:
            UpdateGameplay();
            break;
            
        case GAME_OVER:
            // Press ENTER to restart
            if (IsKeyPressed(KEY_ENTER)) {
                ResetGame();
            }
            break;
    }
}

//------------------------------------------------------------------------------------
// Update Gameplay
//------------------------------------------------------------------------------------
void UpdateGameplay() {
    // Lambda function to update a single player
    auto updatePlayer = [](Player& player) {
        // Horizontal movement
        if (IsKeyDown(player.leftKey)) {
            player.velocity.x = -MOVE_SPEED;
        } else if (IsKeyDown(player.rightKey)) {
            player.velocity.x = MOVE_SPEED;
        } else {
            player.velocity.x = 0;
        }
        
        // Jump
        if (IsKeyPressed(player.jumpKey) && player.isGrounded) {
            player.velocity.y = JUMP_FORCE;
            player.isGrounded = false;
        }
        
        // Apply gravity
        if (!player.isGrounded) {
            player.velocity.y += GRAVITY;
        }
        
        // Update position
        player.rect.x += player.velocity.x;
        player.rect.y += player.velocity.y;
        
        // Screen boundaries (horizontal)
        if (player.rect.x < 0) player.rect.x = 0;
        if (player.rect.x + player.rect.width > SCREEN_WIDTH) {
            player.rect.x = SCREEN_WIDTH - player.rect.width;
        }
        
        // Check ground collision
        player.isGrounded = false;
        for (int i = 0; i < platformCount; i++) {
            if (CheckCollisionRecs(player.rect, platforms[i].rect)) {
                // Simple collision resolution (only from above)
                if (player.velocity.y > 0) {
                    player.rect.y = platforms[i].rect.y - player.rect.height;
                    player.velocity.y = 0;
                    player.isGrounded = true;
                }
            }
        }
        
        // Death condition (fall off screen)
        if (player.rect.y > SCREEN_HEIGHT) {
            return true; // Player died
        }
        
        return false; // Player alive
    };
    
    // Update both players
    bool player1Died = updatePlayer(player1);
    bool player2Died = updatePlayer(player2);
    
    // If any player dies, game over
    if (player1Died || player2Died) {
        currentScreen = GAME_OVER;
        levelCompleted = false;
        return;
    }
    
    // Check if BOTH players reached the exit (cooperative requirement)
    bool player1AtExit = CheckCollisionRecs(player1.rect, exitZone);
    bool player2AtExit = CheckCollisionRecs(player2.rect, exitZone);
    
    if (player1AtExit && player2AtExit) {
        currentScreen = GAME_OVER;
        levelCompleted = true;
    }
}

//------------------------------------------------------------------------------------
// Draw Game (Main draw function)
//------------------------------------------------------------------------------------
void DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    switch(currentScreen) {
        case GAMEPLAY:
            DrawGameplay();
            break;
            
        case GAME_OVER:
            DrawGameOver();
            break;
    }
    
    EndDrawing();
}

//------------------------------------------------------------------------------------
// Draw Gameplay Screen
//------------------------------------------------------------------------------------
void DrawGameplay() {
    // Draw platforms
    for (int i = 0; i < platformCount; i++) {
        DrawRectangleRec(platforms[i].rect, platforms[i].color);
    }
    
    // Draw exit zone
    DrawRectangleRec(exitZone, GREEN);
    DrawText("EXIT", 
             exitZone.x + 20, 
             exitZone.y + 40, 
             20, 
             DARKGREEN);
    
    // Draw players
    DrawRectangleRec(player1.rect, player1.color);
    DrawText("P1", 
             player1.rect.x + 5, 
             player1.rect.y + 10, 
             20, 
             WHITE);
    
    DrawRectangleRec(player2.rect, player2.color);
    DrawText("P2", 
             player2.rect.x + 5, 
             player2.rect.y + 10, 
             20, 
             WHITE);
    
    // UI Instructions
    DrawRectangle(0, 0, SCREEN_WIDTH, 100, Fade(BLACK, 0.7f));
    
    DrawText("CHAOS CREW - Hito 1 Alpha", 20, 10, 30, YELLOW);
    
    DrawText("Player 1: A/D to move, W to jump", 20, 45, 18, BLUE);
    DrawText("Player 2: Arrows to move, UP to jump", 20, 68, 18, RED);
    
    DrawText("COOPERATIVE: Both players must reach the GREEN EXIT!", 
             SCREEN_WIDTH - 550, 
             45, 
             18, 
             GREEN);
    
    // Debug info
    DrawText(TextFormat("FPS: %d", GetFPS()), SCREEN_WIDTH - 100, 10, 20, DARKGRAY);
}

//------------------------------------------------------------------------------------
// Draw Game Over Screen
//------------------------------------------------------------------------------------
void DrawGameOver() {
    // Semi-transparent background
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.8f));
    
    if (levelCompleted) {
        // Victory screen
        DrawText("LEVEL COMPLETE!", 
                 SCREEN_WIDTH/2 - MeasureText("LEVEL COMPLETE!", 60)/2, 
                 SCREEN_HEIGHT/2 - 100, 
                 60, 
                 GREEN);
        
        DrawText("Great teamwork! You both made it!", 
                 SCREEN_WIDTH/2 - MeasureText("Great teamwork! You both made it!", 30)/2, 
                 SCREEN_HEIGHT/2 - 20, 
                 30, 
                 WHITE);
    } else {
        // Failure screen
        DrawText("GAME OVER", 
                 SCREEN_WIDTH/2 - MeasureText("GAME OVER", 60)/2, 
                 SCREEN_HEIGHT/2 - 100, 
                 60, 
                 RED);
        
        DrawText("One player fell! Remember: teamwork is essential!", 
                 SCREEN_WIDTH/2 - MeasureText("One player fell! Remember: teamwork is essential!", 25)/2, 
                 SCREEN_HEIGHT/2 - 20, 
                 25, 
                 WHITE);
    }
    
    DrawText("Press ENTER to try again", 
             SCREEN_WIDTH/2 - MeasureText("Press ENTER to try again", 30)/2, 
             SCREEN_HEIGHT/2 + 50, 
             30, 
             LIGHTGRAY);
    
    // Credits
    DrawText("Chaos Crew - DCA UA 2025", 
             SCREEN_WIDTH/2 - MeasureText("Chaos Crew - DCA UA 2025", 20)/2, 
             SCREEN_HEIGHT - 50, 
             20, 
             GRAY);
}

//------------------------------------------------------------------------------------
// Reset Game
//------------------------------------------------------------------------------------
void ResetGame() {
    InitGame();
}