/*
** Felix Schrynemaekers Project
** ProceduralX
** File description:
** World
*/

#include "World.hpp"

World::World(std::size_t width, std::size_t height, unsigned int seed, SDL_Renderer* renderer)
    : _width(width), _height(height), _seed(seed), _rng(seed), _perlin()
{
    _perlin.SetSeed(seed);
    _perlin.SetFrequency(0.1);
    _perlin.SetPersistence(0.5);
    _perlin.SetLacunarity(2.0);
    _perlin.SetOctaveCount(6);

    _noiseMap.resize(width);
    for (std::size_t i = 0; i < width; ++i) {
        _noiseMap[i].resize(height);
    }

    _grassTexture = loadTexture("../src/assets/grass.png", renderer);
    _sandTexture = loadTexture("../src/assets/sand.png", renderer);
    _waterTexture = loadTexture("../src/assets/water.png", renderer);
    _grassForestTexture = loadTexture("../src/assets/grass_forest.png", renderer);

    loadGrassRocksTextures(renderer);
    loadGrassLeafsTextures(renderer);
    loadSandRocksTextures(renderer);
}


World::~World()
{
    std::cout << "DESTROY WORLD" << std::endl;
}

void World::loadGrassRocksTextures(SDL_Renderer* renderer) {
    _grassRocksTextures.push_back(loadTexture("../src/assets/grass_rocks/grass_rock1.png", renderer));
    _grassRocksTextures.push_back(loadTexture("../src/assets/grass_rocks/grass_rock2.png", renderer));
    _grassRocksTextures.push_back(loadTexture("../src/assets/grass_rocks/grass_rock3.png", renderer));
}

void World::loadGrassLeafsTextures(SDL_Renderer* renderer) {
    _grassLeafsTextures.push_back(loadTexture("../src/assets/grass_leafs/grass_leaf1.png", renderer));
    _grassLeafsTextures.push_back(loadTexture("../src/assets/grass_leafs/grass_leaf2.png", renderer));
    _grassLeafsTextures.push_back(loadTexture("../src/assets/grass_leafs/grass_leaf3.png", renderer));
}

void World::loadSandRocksTextures(SDL_Renderer* renderer) {
    _sandRocksTextures.push_back(loadTexture("../src/assets/sand_rocks/sand_rock1.png", renderer));
    _sandRocksTextures.push_back(loadTexture("../src/assets/sand_rocks/sand_rock2.png", renderer));
    _sandRocksTextures.push_back(loadTexture("../src/assets/sand_rocks/sand_rock3.png", renderer));
}

SDL_Texture* World::loadTexture(const char* filepath, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filepath);
    if (!texture) {
        SDL_Log("Failed to load texture %s: %s", filepath, SDL_GetError());
    }
    return texture;
}


void World::generate() {
    generateBiome();
    // generateRivers();
}

void World::generateBiome() {
    for (int x = 0; x < ( _width - 1); ++x) {
        for (int y = 0; y < _height; ++y) {
            _noiseMap[x][y] = _perlin.GetValue(x, y, 0); 
        }
    }
}

void World::generateRivers() {

}

void World::render(SDL_Renderer* renderer, SDL_Point camera) {    
    int startX = std::max(0, camera.x / TILE_SIZE);
    int startY = std::max(0, camera.y / TILE_SIZE);
    int endX = std::min(_width, (camera.x + _width) / TILE_SIZE + 1);
    int endY = std::min(_height, (camera.y + _height) / TILE_SIZE + 1);

    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {
            double noiseValue = _perlin.GetValue(x / 100.0, y / 100.0, 0.0);

            SDL_Color color; 

            if (noiseValue < -0.2) {
                color = {0, 0, 139, 255}; // Bleu foncé (océan profond)
            }
            else if (noiseValue < 0.0) {
                color = {255, 255, 0, 255}; // Bleu plus clair (océan peu profond)
            }
            else if (noiseValue < 0.3) {
                color = {34, 139, 34, 255}; // Vert (forêt ou plaine)
            }
            else if (noiseValue < 0.6) {
                color = {169, 169, 169, 255}; // Marron (montagnes)
            }
            else {
                color = {255, 255, 255, 255}; // Blanc (neige)
            }

            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

            SDL_FRect rect = {
                static_cast<float>(x * TILE_SIZE - camera.x), 
                static_cast<float>(y * TILE_SIZE - camera.y), 
                static_cast<float>(TILE_SIZE), 
                static_cast<float>(TILE_SIZE)
            };

            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
            SDL_RenderRect(renderer, &rect);
        }
    }
}



