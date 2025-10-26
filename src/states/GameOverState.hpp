#pragma once
#include "../core/GameState.hpp"
#include "GameplayState.hpp"


class GameOverState : public GameState {
public:
    GameOverState(bool victory);
    ~GameOverState() override = default;

    void init() override {}
    void handleInput() override;
    void update(float deltaTime) override {}
    void render() override;
    void pause() override {}
    void resume() override {}

private:
    bool levelCompleted;
};
