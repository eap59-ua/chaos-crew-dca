#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include "../core/GameState.hpp"
#include <string>

// Forward declaration para evitar dependencia circular
class GameplayState;

class GameOverState : public GameState {
public:
    GameOverState(bool victory, std::string mapPath);
    ~GameOverState() override = default;

    void init() override {}
    void handleInput() override;
    void update(float deltaTime) override {}
    void render() override;
    void pause() override {}
    void resume() override {}

private:
    bool levelCompleted;
    std::string mapPath;
};

#endif // GAMEOVERSTATE_HPP
