/*
** Felix Schrynemaekers Project
** ProceduralX
** File description:
** Game
*/

#pragma once
#include "Window.hpp"
#include "Utils.hpp"
#include "World.hpp"

class Game {
    public:
        Game();
        ~Game();

        void run();
        void moveCamera(keyType event);
    private:
        std::shared_ptr<Window> _window;
};
