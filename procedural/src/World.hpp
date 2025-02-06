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

#define TILE_SIZE 16
#define TREE_SIZE 32
#define ROCK_SIZE 32

enum class Biome { PLAINE, FORET, PLAGE, OCEAN };

enum class Tree { NOTREE, TREE, SMALLTREE, FORESTTREE, SMALLFORESTTREE};
enum class Bush { NOBUSH, BUSH };
enum class Rock { NOROCK, SMALLROCK, ROCK, BIGROCK };

struct TileInfo {
    SDL_Texture* texture;
};

struct Tile {
    Biome biome;
    Tree tree;
    Bush bush;
    Rock rock;
};


class World {
    public:
        World(std::size_t width, std::size_t height, unsigned int seed, SDL_Renderer* renderer);
        ~World();

        void generate();
        void generateBiome();
        void generateFeatures();
        void loadGrassRocksTextures(SDL_Renderer* renderer);
        void loadGrassLeafsTextures(SDL_Renderer* renderer);
        void loadSandRocksTextures(SDL_Renderer* renderer);
        SDL_Texture* getTileTexture(Biome biome);
        SDL_Texture* getTreeTexture(Tree tree);
        SDL_Texture* getBushTexture(Bush bush);
        SDL_Texture* getRockTexture(Rock rock);
        SDL_Texture* loadTexture(const char* filepath, SDL_Renderer* renderer);
        void render(SDL_Renderer *renderer, SDL_Point camera);
        void renderFeatures(SDL_Renderer* renderer, SDL_Point camera);

    private:
        std::size_t _width;
        std::size_t _height;
        unsigned int _seed;
        std::mt19937 _rng;
        noise::module::Perlin _perlin;
        noise::module::Voronoi _voronoi;
        std::vector<std::vector<double> > _noiseMap;
        std::vector<SDL_Texture*> _grassRocksTextures;
        std::vector<SDL_Texture*> _grassLeafsTextures;
        std::vector<SDL_Texture*> _sandRocksTextures;
        std::vector<std::vector<Tile> > _tiles;

        SDL_Texture* _grassTexture;
        SDL_Texture* _sandTexture;
        SDL_Texture* _waterTexture;
        SDL_Texture* _grassForestTexture;

        SDL_Texture* _treeTexture;
        SDL_Texture* _smalltreeTexture;
        SDL_Texture* _foresttreeTexture;
        SDL_Texture* _forestsmalltreeTexture;

        SDL_Texture* _bushTexture;

        SDL_Texture* _smallrockTexture;
        SDL_Texture* _rockTexture;
        SDL_Texture* _bigRockTexture;
};
