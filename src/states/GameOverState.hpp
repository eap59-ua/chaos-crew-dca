#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include "../core/GameState.hpp"

// Forward declaration para evitar dependencia circular
class GameplayState;

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

#endif // GAMEOVERSTATE_HPP
