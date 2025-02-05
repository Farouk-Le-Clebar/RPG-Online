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
#include <cstddef>
#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <noise/noise.h>

#define TILE_SIZE 10


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
        void loadGrassRocksTextures(SDL_Renderer* renderer);
        void loadGrassLeafsTextures(SDL_Renderer* renderer);
        void loadSandRocksTextures(SDL_Renderer* renderer);
        SDL_Texture* loadTexture(const char* filepath, SDL_Renderer* renderer);
        void render(SDL_Renderer *renderer, SDL_Point camera);

    private:
        std::size_t _width;
        std::size_t _height;
        unsigned int _seed;
        std::mt19937 _rng;
        noise::module::Perlin _perlin;
        std::vector<std::vector<double> > _noiseMap;
        std::vector<SDL_Texture*> _grassRocksTextures;
        std::vector<SDL_Texture*> _grassLeafsTextures;
        std::vector<SDL_Texture*> _sandRocksTextures;

        SDL_Texture* _grassTexture;
        SDL_Texture* _sandTexture;
        SDL_Texture* _waterTexture;
        SDL_Texture* _grassForestTexture;
};
