/*
** Felix Schrynemaekers Project
** ProceduralX
** File description:
** Window
*/

#pragma once
#include "Utils.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstddef>

class Window {
    public:
        Window();
        ~Window();

        void init();
        eventType updateEvents();
        keyType getInput();
        void moveCamera(int x, int y);
        void clearWindow();
        void render();
        SDL_Point getCamera() { return _camera; }
        SDL_Renderer *getRenderer() { return _renderer; }
        void destroyWindow();
    private:
        std::size_t _windowWidth;
        std::size_t _windowHeight;
        SDL_Window *_window;
        SDL_Renderer *_renderer;
        SDL_Event _event;
        SDL_Point _camera;
        std::unordered_map<SDL_Scancode, bool> _keyStates;
};