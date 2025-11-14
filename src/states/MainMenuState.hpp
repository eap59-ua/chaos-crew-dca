#ifndef MAINMENU_HPP
#define  MAINMENU_HPP

#pragma once
#include "../core/GameState.hpp"

class MainMenuState : public GameState {
public:
    MainMenuState() = default;
    ~MainMenuState() override = default;

    void init() override {}
    void handleInput() override;
    void update(float dt) override {}
    void render() override;
    void pause() override {}
    void resume() override {}
};


#endif