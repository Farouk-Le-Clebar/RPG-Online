/*
** Felix Schrynemaekers Project
** ProceduralX
** File description:
** Window
*/

#include "Window.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

Window::Window() {
  _camera = {0, 0};
}

Window::~Window() {}

void Window::init() {
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    exit(84);
  }

  if (!SDL_Init(SDL_INIT_AUDIO)) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
  }

  if (!TTF_Init()) {
    std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
    exit(84);
  }
  SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
  const SDL_DisplayMode *currentMode = SDL_GetCurrentDisplayMode(displayID);

  _windowWidth = 1200;
  _windowHeight = 800;

  _window = SDL_CreateWindow("ProceduralX", _windowWidth, _windowHeight, 0);
  if (!_window) {
    std::cerr << "Erreur lors de la création de la fenêtre : " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    exit(84);
  }

  _renderer = SDL_CreateRenderer(_window, nullptr);
  if (!_renderer) {
    std::cerr << "Erreur lors de la création du renderer : " << SDL_GetError()
              << std::endl;
    destroyWindow();
    exit(84);
  }
}

eventType Window::updateEvents() {
    while (SDL_PollEvent(&_event)) {
        if (_event.type == SDL_EVENT_QUIT) {
            return CLOSE_WINDOW;
        }
        if (_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            return MOUSE_CLICK;
        }
        if (_event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            return MOUSE_RELEASE;
        }
        if (_event.type == SDL_EVENT_KEY_DOWN) {
            _keyStates[_event.key.scancode] = true;
            return KEY_DOWN;
        }
        if (_event.type == SDL_EVENT_KEY_UP) {
            _keyStates[_event.key.scancode] = false;
        }
    }
    return NO_EVENT;
}

keyType Window::getInput() {
    if (_keyStates[SDL_SCANCODE_UP]) {
        return UP;
    }
    if (_keyStates[SDL_SCANCODE_DOWN]) {
        return DOWN;
    }
    if (_keyStates[SDL_SCANCODE_LEFT]) {
        return LEFT;
    }
    if (_keyStates[SDL_SCANCODE_RIGHT]) {
        return RIGHT;
    }
    return NONE;
}

void Window::moveCamera(int x, int y) {
    _camera.x += x;
    _camera.y += y;
}


void Window::clearWindow() {
  SDL_SetRenderDrawColor(_renderer, 21, 21, 21, 255);
  SDL_RenderClear(_renderer);
}

void Window::render() {
  SDL_RenderPresent(_renderer);
}

void Window::destroyWindow() {
  SDL_DestroyWindow(_window);
  TTF_Quit();
  SDL_Quit();
}