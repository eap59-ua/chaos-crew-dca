#ifndef PAUSESTATE_HPP
#define  PAUSESTATE_HPP

#include "../core/GameState.hpp"

class PauseState : public GameState {
public:
    PauseState() = default;
    ~PauseState() override = default;

    void init() override {}
    void handleInput() override;
    void update(float deltatime) override {}
    void render() override;
    void pause() override {}
    void resume() override {}
};

#endif