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

    _tileTextures.resize(_width, std::vector<TileInfo>(_height));

    _grassTexture = loadTexture("../src/assets/grass.png", renderer);
    _sandTexture = loadTexture("../src/assets/sand.png", renderer);
    _waterTexture = loadTexture("../src/assets/watter.png", renderer);
    _grassForestTexture = loadTexture("../src/assets/grass_forest.png", renderer);

    _treeTexture = loadTexture("../src/assets/tree.png", renderer);
    _smalltreeTexture = loadTexture("../src/assets/smalltree.png", renderer);
    _foresttreeTexture = loadTexture("../src/assets/forest_tree.png", renderer);
    _forestsmalltreeTexture = loadTexture("../src/assets/forest_smalltree.png", renderer);

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
    std::uniform_int_distribution<int> grassLeafDist(0, _grassLeafsTextures.size() - 1);
    std::uniform_int_distribution<int> sandRocksDist(0, _sandRocksTextures.size() - 1);

    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            double noiseValue = _perlin.GetValue(x / 100.0, y / 100.0, 0.0);
            _noiseMap[x][y] = noiseValue;

            if (noiseValue < -0.2) {
                _tileTextures[x][y].texture = _waterTexture;
            }
            else if (noiseValue < 0.0) {
                std::uniform_int_distribution<int> dist(0, 15);
                if (dist(_rng) == 0) {
                    _tileTextures[x][y].texture = _sandRocksTextures[sandRocksDist(_rng)];
                } else {
                    _tileTextures[x][y].texture = _sandTexture;
                }
            }
            else if (noiseValue < 0.3) {
                std::uniform_int_distribution<int> dist(0, 10);
                if (dist(_rng) == 0) {
                    _tileTextures[x][y].texture = _grassLeafsTextures[grassLeafDist(_rng)];
                } else {
                    _tileTextures[x][y].texture = _grassTexture;
                }
            }
            else if (noiseValue < 0.5) {
                _tileTextures[x][y].texture = _grassForestTexture;
            }
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
            SDL_Texture* texture = _tileTextures[x][y].texture;

            SDL_FRect rect = {
                static_cast<float>(x * TILE_SIZE - camera.x), 
                static_cast<float>(y * TILE_SIZE - camera.y), 
                static_cast<float>(TILE_SIZE), 
                static_cast<float>(TILE_SIZE)
            };

            SDL_RenderTexture(renderer, texture, nullptr, &rect);
        }
    }
}





