/*
** Felix Schrynemaekers Project
** ProceduralX
** File description:
** Game
*/

#include "Game.hpp"

Game::Game()
{
    _window = std::make_shared<Window>();
}

Game::~Game()
{
}

void Game::moveCamera(keyType event)
{
    if (event == NONE)
        return;
    if (event == UP) {
        _window->moveCamera(0, -1);
    } else if (event == DOWN) {
        _window->moveCamera(0, 1);
    } else if (event == LEFT) {
        _window->moveCamera(-1, 0);
    } else if (event == RIGHT) {
        _window->moveCamera(1, 0);
    }
}

void Game::run()
{
    unsigned int seed;
    _window->init();
    eventType event = NO_EVENT;
    keyType input = NONE;

    seed = static_cast<unsigned int>(time(nullptr));
    World world(120, 80, seed, _window->getRenderer());
    world.generate();

    while (event != CLOSE_WINDOW) {
        _window->clearWindow();
        event = _window->updateEvents();
        input = _window->getInput();

        moveCamera(input);
        world.render(_window->getRenderer(), _window->getCamera());

        // window system
        _window->render();
    }
    if (event == CLOSE_WINDOW) {
        _window->destroyWindow();
    }
}
