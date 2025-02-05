/*
** Felix Schrynemaekers Project
** ProceduralX
** File description:
** World
*/

#pragma once
#define SDL_DISABLE_OLD_NAMES
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "PerlinNoise.hpp"
#include <cstddef>
#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>

#define TILE_SIZE 32


enum class Biome { PLAINE, FORET, DESERT, RIVIERE, MONTAGNE, OCEAN };

struct Tile {
    Biome biome;
};

class World {
    public:
        World(std::size_t width, std::size_t height, unsigned int seed, SDL_Renderer* renderer);
        ~World();

        void generate();
        void generateBiome();
        void generateRivers();
        SDL_Texture* loadTexture(const char* filepath, SDL_Renderer* renderer);
        void render(SDL_Renderer *renderer, SDL_Point camera);
        float getNoiseValue(std::size_t x, std::size_t y);

    private:
        std::size_t _width;
        std::size_t _height;
        unsigned int _seed;
        std::mt19937 _rng;
        std::vector<std::vector<Tile>> _tiles;
            
        SDL_Texture* desertTexture;
        SDL_Texture* plaineTexture;
        SDL_Texture* foretTexture;
        SDL_Texture* riviereTexture;
        SDL_Texture* montagneTexture;
        SDL_Texture* oceanTexture;

};
