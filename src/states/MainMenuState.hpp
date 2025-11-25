#ifndef MAINMENU_HPP
#define  MAINMENU_HPP

#pragma once
#include "../core/GameState.hpp"
#include <raylib.h>
#include <vector>
#include <string>

using namespace std;
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
    public:
        MainMenuState() = default;
        ~MainMenuState() override = default;

        void init() override;
        void handleInput() override;
        void update(float) override {}
        void render() override;
        void pause() override {}
        void resume() override {}

    private:
        vector<string>maps;
        int selected = 0;
        vector<bool>unlocked;
        vector<bool>completed;

        void rebuildData();
};


#endif