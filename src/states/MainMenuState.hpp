#ifndef MAINMENU_HPP
#define  MAINMENU_HPP

#pragma once
#include "../core/GameState.hpp"
#include <raylib.h>

class MainMenuState : public GameState {
public:
    MainMenuState();
    ~MainMenuState() override;

    void init() override;
    void handleInput() override;
    void update(float dt) override;
    void render() override;
    void pause() override {}
    void resume() override {}
private:
    Music menuMusic;
};


#endif