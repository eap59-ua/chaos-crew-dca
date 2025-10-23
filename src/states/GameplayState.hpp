#pragma once
#include "core/GameState.hpp"
#include "core/StateMachine.hpp"
#include <vector>
#include <raylib.h>


struct Player {
    Rectangle rect;
    Vector2 velocity;
    Color color;
    bool isGrounded;
    int leftKey;
    int rightKey;
    int jumpKey;
};

struct Platform {
    Rectangle rect;
    Color color;
};

class GameplayState : public GameState {
public:
    GameplayState();
    ~GameplayState() override = default;

    void init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;
    void pause() override {}
    void resume() override {}

private:
    Player player1;
    Player player2;
    std::vector<Platform> platforms;
    Rectangle exitZone;
    bool levelCompleted;
    bool isGameOver;
};
