#ifndef PAUSESTATE_HPP
#define  PAUSESTATE_HPP

#pragma once
#include "../core/GameState.hpp"
#include "../core/StateMachine.hpp"
#include <raylib.h>

class PauseState : public GameState {
public:
    explicit PauseState(const std::string& currentMap): selected(0), mapPath(currentMap) {}
    ~PauseState() override = default;

    void init() override {}
    void handleInput() override;
    void update(float) override {}
    void render() override;
    void pause() override {}
    void resume() override {}

private:
    int selected;   // 0=continuar, 1=reiniciar, 2=salir
    std::string mapPath;
};


#endif